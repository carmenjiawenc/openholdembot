#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Game-state class for the table
//   (common cards and players)
//
//******************************************************************************

#include "Card.h"
#include "CPlayer.h"
#include "CScrapedMoney.h"
#include "CTableTitle.h"
#include "LibDef.h"
#include "SLimitInfo.h"

// One fake-entry for the case of unknown user-chair
// This way we can avoid the handling of special cases
// and the danger of NULL-pointers for the User()-function.
const int kNumberOfPlayerEntries = kMaxNumberOfPlayers + 1;
const int kFakeEntryForUnknownUserchair = kMaxNumberOfPlayers;

class TABLESTATE_DLL_API CTableState {
public:
  CTableState();
  ~CTableState();
public:
  void Reset();
public:
  CPlayer *User();
  CPlayer *Player(int chair);
  double	Pot(int sidepot_index);
public:
  Card *CommonCards(int common_card_index);
  Card *TurnCard();
  Card *RiverCard();
  int  NumberOfCommunityCards();
  // For CHandresetDetector
  bool ShowdownCardsVisible();
  bool AntesVisible();
public:
  CTableTitle *TableTitle();
public:
  bool set_pot(int sidepot_index, CString new_value);
  void ResetPots();
public:
  // Data-container for input from scraper
  // needs to be verified / extended by symbol-engines
  CLimitInfo _s_limit_info;
private:
  CPlayer _players[kNumberOfPlayerEntries];
  Card    _common_cards[kNumberOfCommunityCards];
  CScrapedMoney _pot[kMaxNumberOfPots];
private:
  CTableTitle _table_title;
};

TABLESTATE_DLL_API CTableState* TableState();