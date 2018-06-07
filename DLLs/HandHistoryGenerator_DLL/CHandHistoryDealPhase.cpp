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

#include "CHandHistoryDealPhase.h"
#include "CHandHistoryWriter.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Symbols_DLL\CBetroundCalculator.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineActiveDealtPlaying.h"
#include "..\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\Symbols_DLL\CSymbolEngineDealerchair.h"
#include "..\Symbols_DLL\CSymbolEngineTableLimits.h"
#include "..\Tablestate_DLL\TableState.h"

CHandHistoryDealPhase::CHandHistoryDealPhase() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	/*#assert(EngineContainer()->symbol_engine_active_dealt_playing() != NULL);
  assert(EngineContainer()->symbol_engine_chip_amounts() != NULL);
  assert(EngineContainer()->symbol_engine_dealerchair() != NULL);
  assert(EngineContainer()->symbol_engine_tablelimits() != NULL);*/
  // No dependency to CHandHistoryWriter as this modules
  // does not compute any symbols but collects our data.
}

CHandHistoryDealPhase::~CHandHistoryDealPhase() {
}

void CHandHistoryDealPhase::InitOnStartup() {
}

void CHandHistoryDealPhase::UpdateOnConnection() {
}

void CHandHistoryDealPhase::UpdateOnHandreset() {
  _job_done = false;
}

void CHandHistoryDealPhase::UpdateOnNewRound() {
}

void CHandHistoryDealPhase::UpdateOnMyTurn() {
}

void CHandHistoryDealPhase::UpdateOnHeartbeat() {
  /*#
  if (_job_done) return;
///  if (BETROUND > kBetroundPreflop) {
    // Can only happen when we join a table
    // in the middle of a hand
    _job_done = true;
    return;
  ///}
  if (EngineContainer()->symbol_engine_active_dealt_playing()->nopponentsdealt() < 1 ) {
    // Blind-Posting not yet finished
    return;
  }
  // At least 1 player dealt
  // Now we know everybody who posted a blind or ante
  // Searching clockwise for blind posters
  bool smallblind_seen = false;
  bool bigblind_seen   = false;
  int last_chair  = EngineContainer()->symbol_engine_dealerchair()->dealerchair();
  int first_chair = (last_chair + 1) % BasicScraper()->Tablemap()->nchairs();
  for (int i=first_chair; i<=last_chair; ++i) {
    double currentbet = TableState()->Player(i)->_bet.GetValue();
    if (currentbet <= 0) {
      // Not having to post, not posting or not participating at all
      continue;
    }
    if (smallblind_seen && bigblind_seen) {
      if (currentbet < EngineContainer()->symbol_engine_tablelimits()->sblind()) {
        ///p_handhistory_writer->PostsAnte(i);
      }
      // We ignore additional people with a bigblind
      // They are maybe additional blind-posters
      // but in general can't be distinguished from callers
      continue;
    }
    if (smallblind_seen) {
      assert(!bigblind_seen);
      // Player must be posting the bigblind
      ///p_handhistory_writer->PostsBigBlind(i);
      bigblind_seen = true;
      continue;
    }
    // Otherwise> not yet blinds seen
    // Usually a smallblind
    // Might be also a bigblind with missing small blind
    assert(currentbet > 0);
    if (currentbet <= EngineContainer()->symbol_engine_tablelimits()->sblind()) {
      ///p_handhistory_writer->PostsSmallBlind(0);
      smallblind_seen = true;
      continue;
    }
    if ((currentbet > EngineContainer()->symbol_engine_tablelimits()->sblind()) 
        && (currentbet <= EngineContainer()->symbol_engine_tablelimits()->sblind())) {
      // Bigblind and missing smallblind
      ///p_handhistory_writer->PostsBigBlind(i);
      smallblind_seen = true;
      bigblind_seen = true;
      continue;
    }
  }*/
}

bool CHandHistoryDealPhase::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryDealPhase::SymbolsProvided() {
  // No symbols provided
  return " ";
}
	