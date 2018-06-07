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

#include "CHandHistoryShowdown.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Symbols_DLL\CBetroundCalculator.h"
///#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineUserchair.h"
#include "..\Tablestate_DLL\TableState.h"

CHandHistoryShowdown::CHandHistoryShowdown() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	///assert(EngineContainer()->symbol_engine_userchair() != NULL);
  UpdateOnHandreset();
}

CHandHistoryShowdown::~CHandHistoryShowdown() {
}

void CHandHistoryShowdown::InitOnStartup() {
}

void CHandHistoryShowdown::UpdateOnConnection() {
}

void CHandHistoryShowdown::UpdateOnHandreset() {
  _job_done = false;
  _river_seen = false;
}

void CHandHistoryShowdown::UpdateOnNewRound() {
}

void CHandHistoryShowdown::UpdateOnMyTurn() {
}

bool AnyOpponentsCardsVisible() {
  for (int i=0; i<BasicScraper()->Tablemap()->nchairs(); ++i) {
    ///if (i == EngineContainer()->symbol_engine_userchair()->userchair()) {
      // Not an opponent
      continue;
    ///}
    if (TableState()->Player(i)->HasKnownCards()) {
      return true;
    }
  }
  return false;
}

bool AllPlayersActed() {
  for (int i=0; i<BasicScraper()->Tablemap()->nchairs(); ++i) {

  }
  return false;
}

void CHandHistoryShowdown::UpdateOnHeartbeat() {
///  if (BetroundCalculator()->betround() == kBetroundRiver) {
    _river_seen = true;
  ///}
  if (!_river_seen) return;
  // For sure showdown if
  // * somebody else shows his cards
  // * or if all players have a non-zero equal bet or are allin
  // But none of the conditions above is guaranteed to be true:
  // * the user could be showing cards, while others ,uck.
  // * cards could be highlightes (not recogniyed)
  // * the river could be check (impossible to distinguish from no action)
  if (!(AnyOpponentsCardsVisible() || AllPlayersActed())) return;
  _job_done = true;
}

bool CHandHistoryShowdown::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryShowdown::SymbolsProvided() {
  // No symbols provided
  return " ";
}
	