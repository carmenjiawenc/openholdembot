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

#include "CHandHistoryUncontested.h"
#include "CHandHistoryWriter.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineActiveDealtPlaying.h"
#include "..\Tablestate_DLL\TableState.h"

CHandHistoryUncontested::CHandHistoryUncontested() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  ///assert(EngineContainer()->symbol_engine_active_dealt_playing() != NULL);
	_job_done = false;
}

CHandHistoryUncontested::~CHandHistoryUncontested() {
}

void CHandHistoryUncontested::InitOnStartup() {
}

void CHandHistoryUncontested::UpdateOnConnection() {
}

void CHandHistoryUncontested::UpdateOnHandreset() {
  _job_done = false;
}

void CHandHistoryUncontested::UpdateOnNewRound() {
}

void CHandHistoryUncontested::UpdateOnMyTurn() {
}

void CHandHistoryUncontested::UpdateOnHeartbeat() {
  if (_job_done) return;
  ///if (EngineContainer()->symbol_engine_active_dealt_playing()->nplayersdealt() < 2) return;
  ///if (EngineContainer()->symbol_engine_active_dealt_playing()->nplayersactive() != 1) return;
  // So here we are:
  // * a game is going on
  // * only 1 player is left, who wins uncontested
  for (int i=0; i<BasicScraper()->Tablemap()->nchairs(); ++i) {
    ///if (IsBitSet(EngineContainer()->symbol_engine_active_dealt_playing()->playersactivebits(), i)) {
      CString message;
      message.Format("Player %s wins the pot uncontested\n",
        TableState()->Player(i)->name());
      ///p_handhistory_writer->AddMessage(message);
      _job_done = true;
      return;
    ///}
  }
}

bool CHandHistoryUncontested::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryUncontested::SymbolsProvided() {
  // No symbols provided
  return " ";
}
	