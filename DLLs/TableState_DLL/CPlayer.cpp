//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#define TABLESTATE_DLL_EXPORTS

#include "CPlayer.h"
#include "..\Numerical_Functions_DLL\FloatingPoint_Comparisions.h"
#include "..\OpenHoldem_CallBack_DLL\OpenHoldem_CallBack.h"

// Blobal dummy to handle access to non-existing cards easily
Card dummy_card_nocard;

CPlayer::CPlayer() {
}

CPlayer::~CPlayer() {
}

void CPlayer::Reset() {
  set_name("");
  _balance.Reset();
  _bet.Reset();
  for (int i=0; i<kMaxNumberOfCardsPerPlayer; ++i) {
    _hole_cards[i].ClearValue();
  }
  // We can't use set_seated(false) here, as set_seated(false) 
  // will call Reset() again; endless recursion
  _seated = false;
  set_active(false);
  set_dealer(false);
  set_colourcode(kUndefinedZero);
}

bool CPlayer::HasAnyCards() {
  // We do no longer check for cardbacks,
	// but for cardbacks or cards.
	// This way we can play all cards face-up at PokerAcademy.
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=13384
  for (int i = 0; i < 2 /*!!!!!!!NumberOfCardsPerPlayer()*/; ++i) {
    if (!_hole_cards[i].IsAnyCard()) {
      return false;
    }
  }
  return true;
}

CString CPlayer::Cards() {
	// log new hand
  if (HasKnownCards()) {
    CString result; 
    for (int i = 0; i < 2 /*!!!!!!!NumberOfCardsPerPlayer()*/; ++i) {
      result += _hole_cards[i].ToString();
    }
    return result;
  }
  if (HasAnyCards()) {
    return "BACKS";
  }
  return "NONE";
}

CString CPlayer::CardsAsHTML() {
  // Returning ranks and suits (symbols, colourful) in case of cards
  // Returning "**" on case of card-backs
  // Returning non-breaking spaces in case of no cards
  CString result;
  for (int i = 0; i < 2 /*!!!!!!!NumberOfCardsPerPlayer()*/; ++i) {
    result += hole_cards(i)->ToHTML();
  }
  return result;
}

bool CPlayer::HasKnownCards() {
  for (int i = 0; i < 2/*!!!!!!!NumberOfCardsPerPlayer()*/; ++i) {
    if (!_hole_cards[i].IsKnownCard()) {
      return false;
    }
  }
  return true;
}

Card* CPlayer::hole_cards(int index) {
  // Make sure there is no index overflop
  // and be extra careful, acce4ss may depend on user-input
  if (index < 0) {
    return &dummy_card_nocard;
  }
  if (index >= kMaxNumberOfCardsPerPlayer) {
    return &dummy_card_nocard;
  }
  return &_hole_cards[index];
}

void CPlayer::CheckPlayerCardsForConsistency() {
  if (HasKnownCards() && (_hole_cards[0].GetValue() == _hole_cards[1].GetValue())) {
    // Identical cards, something clearly went wrong.
    // Something "known", not "nocard" and not "undefined",
    // but clearly wrong (duplicate card).
    // We assume, that we see something that differs from the background,
    // probably cardbacks, that sometimes get scraped as JJsuited or 88suited
    // by bad tablemaps.
    // Above check was designed for HoldEm, and we keep it that way,
    // as we don't want to consider any error-correction if 3 of 4 cards are good.
    _hole_cards[0].SetValue(CARD_BACK);
    _hole_cards[1].SetValue(CARD_BACK);
    _hole_cards[2].SetValue(CARD_BACK);
    _hole_cards[3].SetValue(CARD_BACK);
  }
}

bool CPlayer::IsAllin() {
  return (_seated && _active && HasAnyCards() && (_balance.GetValue() == 0));
}

bool CPlayer::PostingBothBlinds() {
  if (EvaluateSymbol("betround") > kBetroundPreflop*/) {
    // No blind posters postflop
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19043
    return false;
  }
  if (IsAllin()) {
    // A person who is allin for SB + BB can't be new at the table, 
    // therefore not posting both blinds.
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19414
    return false;
  }
  // We have to calculate in cents here, as IsApproximatellyEqual uses rounding internally.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=18743
  double bet_in_cents = 100 * _bet.GetValue();
  double both_blinds_in_cents = 100 * 
    (EvaluateSymbol("sblind") + EvaluateSymbol("bblind"));
  return (_seated && _active && HasAnyCards()
    && IsApproximatellyEqual(bet_in_cents, both_blinds_in_cents));
}

bool CPlayer::PostingAnte() {
  if (EvaluateSymbol("betround") > kBetroundPreflop) {
    // No ante posters postflop
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19043
    return false;
  }
  if (IsAllin()) {
    // A person who is allin for SB + BB can't be new at the table, 
    // therefore not posting antes.
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19414
    return false;
  }
  if (_bet.GetValue() <= 0) {
    return false;
  }
  if (_bet.GetValue() >= EvaluateSymbol("sblind")) {
    // Assuming the ante is smaller as the small blind
    // There are exceptions, but we use this function 
    // only as one of many handreset-methods.
    return false;
  }
  return true;
}

void CPlayer::set_seated(bool is_seated) {
  if ((is_seated == false)) {
    // Change from seated to non-seated
    // We should clear all player data in this case
    // as the lazy-scraper stops on empty seats
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=20567
    Reset();
  }
  _seated = is_seated;
}

void CPlayer::set_seated(CString scraped_info) {
  // Check for bad parameters
  if (!scraped_info || scraped_info == "") {
    return;
  }
  CString s_lower_case = scraped_info;
  s_lower_case.MakeLower();
  if (s_lower_case.Left(5) == "false" || s_lower_case.Left(8) == "unseated") {
    set_seated(false);
    return;
  }
  if (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "seated") {
    set_seated(true);
  }
}

void CPlayer::set_active(CString scraped_info) {
  // Check for bad parameters
  if (!scraped_info || scraped_info == "") {
    return;
  }
  CString s_lower_case = scraped_info;
  s_lower_case.MakeLower();
  if (s_lower_case.Left(5) == "false"
    || s_lower_case.Left(8) == "inactive"
    || s_lower_case.Left(3) == "out"
    || s_lower_case.Left(6) == "sitout"
    || s_lower_case.Left(4) == "away") {
    set_active(false);
    return;
  }
  // old method: inactive unless pXactive returns true/active
  if (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "active") {
    set_active(true);
  }
}

void CPlayer::set_dealer(CString scraped_info) {
  // Check for bad parameters
  if (!scraped_info || scraped_info == "") {
    return;
  }
  CString s_lower_case = scraped_info;
  s_lower_case.MakeLower();
  if (s_lower_case.Left(4) == "true"
    || s_lower_case.Left(6) == "dealer") {
    set_dealer(true);
  }
}

CString CPlayer::DataDump() {
  CString result = "";
  // 20 digits, right alighned
  result.Format("%20s", name());
  result += "  ";
  // Seated, active, playing, dealer
  if (seated()) {
    result += "S";
  } else {
    result += "-";
  }
  if (active()) {
    result += "A";
  }  else {
    result += "-";
  }
  if (HasAnyCards()) {
    result += "P";
  } else {
    result += "-";
  }
  if (dealer()) {
    result += "D";
  } else {
    result += "-";
  }
  result += "  ";
  // Money: currentbet and balance
  CString money;
  money.Format("%12.2f  %12.2f", _bet.GetValue(), _balance.GetValue());
  result += money;
  return result;
}