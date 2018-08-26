//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Symbol lookup for various symbols 
//   that are not part of a regular symbol-engine
//
//******************************************************************************

#include "CSymbolEngineCasino.h"
#include <assert.h>
#include <process.h>
#include <float.h>
#include "Chair$Symbols.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\SessionCounter_DLL\CSessionCounter.h"
#include "..\WindowFunctions_DLL\window_functions.h"

CSymbolEngineCasino::CSymbolEngineCasino() {
  // The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineCasino::~CSymbolEngineCasino() {
}

void CSymbolEngineCasino::InitOnStartup() {
}

void CSymbolEngineCasino::UpdateOnConnection() {
}

void CSymbolEngineCasino::UpdateOnHandreset() {
}

void CSymbolEngineCasino::UpdateOnNewRound() {
}

void CSymbolEngineCasino::UpdateOnMyTurn() {
}

bool CSymbolEngineCasino::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  // CHAIRS 1(2)
  if (memcmp(name, "chair", 5)==0) {
    if (memcmp(name, "chair$", 6)==0)							*result = Chair$(name.Mid(6));
    else if (memcmp(name, "chairbit$", 9)==0)					*result = Chairbit$(name.Mid(9));
    else return false;
  }
  // GENERAL
  else if (memcmp(name, "nchairs", 7) == 0 && strlen(name) == 7) *result = BasicScraper()->Tablemap()->nchairs();
  //PROFILE
  ///else if (memcmp(name, "sitename$", 9)==0)	*result = SitenameContainsCasinoIdentifier(name.Mid(9));
  else if (memcmp(name, "network$", 8)==0)	*result = BasicScraper()->Tablemap()->network().Find(name.Mid(8))!=-1;
  else {
    *result = kUndefined;
    return false;
  }
  return true;
}

CString CSymbolEngineCasino::SymbolsProvided() {
  // This list includes some prefixes of symbols that can't be verified,
  return "chair$ chairbit$ sitename$ network$ nchairs ";
}

void CSymbolEngineCasino::UpdateOnHeartbeat() {
}

