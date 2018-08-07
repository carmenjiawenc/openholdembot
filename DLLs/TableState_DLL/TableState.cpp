//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Game-state object for the table
//   (common cards and players)
//
//******************************************************************************

#define TABLESTATE_DLL_EXPORTS

#include "TableState.h"
#include "..\Numerical_Functions_DLL\NumericalFunctions.h"
#include "..\OpenHoldem_CallBack_DLL\OpenHoldem_CallBack.h"

/*!!!!!!!#include "CEngineContainer.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineActiveDealtPlaying.h"*/

CTableState::CTableState() {
  Reset();
}

CTableState::~CTableState() {
}

void CTableState::Reset() {
  _s_limit_info.Reset();
  for (int i = 0; i<kNumberOfCommunityCards; ++i) {
    _common_cards[i].ClearValue();
  }
  for (int i = 0; i<kNumberOfPlayerEntries; ++i) {
    Player(i)->Reset();
  }
  for (int i = 0; i<kMaxNumberOfPots; ++i) {
    _pot[i].Reset();
  }
  TableTitle()->Clear();
}

CPlayer *CTableState::User() {
  /*!!!!!!!if (EngineContainer()->symbol_engine_userchair() == NULL) {
    return &_players[kFakeEntryForUnknownUserchair];
  }
  if (!EngineContainer()->symbol_engine_userchair()->userchair_confirmed()) {
    return &_players[kFakeEntryForUnknownUserchair];
  }*/
  int userchair = EvaluateSymbol("userchair");
  AssertRange(userchair, 0, kLastChair);
  ///!!! handle fake-entry
  return Player(userchair);
}

CPlayer *CTableState::Player(int chair) {
  assert(chair >= 0);
  assert(chair < kNumberOfPlayerEntries);
  return &_players[chair];
}

double CTableState::Pot(int sidepot_index) {
  AssertRange(sidepot_index, 0, kLastPot);
  return  _pot[sidepot_index].GetValue();
}

bool CTableState::set_pot(int sidepot_index, CString new_value) {
  AssertRange(sidepot_index, 0, kLastPot);
  return _pot[sidepot_index].SetValue(new_value);
}

void CTableState::ResetPots() {
  for (int j = 0; j < kMaxNumberOfPots; j++) {
    _pot[j].Reset();
  }
}

Card *CTableState::CommonCards(int common_card_index) {
  AssertRange(common_card_index, 0, kIndexOfLastCommunityCard);
  return &_common_cards[common_card_index];
}

Card *CTableState::TurnCard() {
  return CommonCards(kIndexOfTurnCard);
}

Card *CTableState::RiverCard() {
  return CommonCards(kIndexOfRiverCard);
}

int CTableState::NumberOfCommunityCards() {
  int result = 0;
  for (int i = 0; i < kNumberOfCommunityCards; ++i) {
    if (_common_cards[i].IsKnownCard()) {
      ++result;
    }
    else {
      break;
    }
  }
  return result;
}

bool CTableState::ShowdownCardsVisible() {
  for (int i = 0; i < kMaxNumberOfPlayers; ++i) {
    if (&_players[i] == User()) {
      // user-cards, probably no showdown
      continue;
    }
    if (Player(i)->HasKnownCards()) {
      // Opponent with visible cards, usually showdown.
      // Might also be a poker-simulatpr sh owing all players face-cards
      // Might also be new user-chair after table-change in MTT, ...
      // but thenwe had a handreset, all OK.
      return false;
    }
  }
  return false;
}

bool CTableState::AntesVisible() {
  int number_of_players_posting_ante = 0;
  for (int i = 0; i < kMaxNumberOfPlayers; ++i) {
    if (Player(i)->PostingAnte()) {
      ++number_of_players_posting_ante;
    }
  }
  // Be conservatibe, aboid scraping errors
  // Require more than one player with a tiny bet
  return (number_of_players_posting_ante > 1);
}

CTableTitle *CTableState::TableTitle() {
  return &_table_title;
}

CTableState* table_state = NULL;

CTableState* TableState() {
  if (table_state == NULL) {
    // Lazy initialization
    table_state = new CTableState;
  }
  return table_state;
}