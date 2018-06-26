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

#include "CSymbolEngineVariousDataLookup.h"
#include <assert.h>
#include <process.h>
#include <float.h>
#include "CBetroundCalculator.h"
#include "CEngineContainer.h"
#include "inlines/eval.h"
#include "Chair$Symbols.h"
#include "CSymbolEngineUserchair.h"
#include "..\Debug_DLL\debug.h"
#include "..\Formula_DLL\CFormulaParser.h"
#include "..\Globals_DLL\globals.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\Scraper_DLL\CTransform\CTransform.h"
#include "..\SessionCounter_DLL\CSessionCounter.h"
#include "..\TableManagement_DLL\CAutoConnector.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\Tablestate_DLL\TableState.h"
#include "..\Tablestate_DLL\CTableTitle.h"
#include "..\WindowFunctions_DLL\window_functions.h"

CSymbolEngineVariousDataLookup *p_symbol_engine_various_data_lookup = NULL;

CSymbolEngineVariousDataLookup::CSymbolEngineVariousDataLookup() {
  // The values of some symbol-engines depend on other engines.
  // As the engines get later called in the order of initialization
  // we assure correct ordering by checking if they are initialized.
  assert(EngineContainer()->symbol_engine_userchair() != NULL);
  // Other objects that we depend on
  assert(TableManagement()->AutoConnector() != NULL);
  assert(p_betround_calculator != NULL);
  assert(OpenHoldem()->HandresetDetector() != NULL);
  assert(SessionCounter() != NULL);
  assert(p_tablemap != NULL);
  assert(TableState()->TableTitle() != NULL);
}

CSymbolEngineVariousDataLookup::~CSymbolEngineVariousDataLookup() {
}

void CSymbolEngineVariousDataLookup::InitOnStartup() {
}

void CSymbolEngineVariousDataLookup::UpdateOnConnection() {
}

void CSymbolEngineVariousDataLookup::UpdateOnHandreset() {
  // Reset display
  ///!!!!! not here -> heartbeat
  ///InvalidateRect(theApp.m_pMainWnd->GetSafeHwnd(), NULL, true);
}

void CSymbolEngineVariousDataLookup::UpdateOnNewRound() {
}

void CSymbolEngineVariousDataLookup::UpdateOnMyTurn() {
}

void CSymbolEngineVariousDataLookup::UpdateOnHeartbeat() {
}

bool CSymbolEngineVariousDataLookup::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  // Various symbols below
  // without any optimized lookup.
  // Betting rounds
  if (memcmp(name, "betround", 8)==0 && strlen(name)==8)	*result = BetroundCalculator()->betround();
  else if (name == "currentround") *result = BetroundCalculator()->betround();
  else if (name == "previousround") *result = BetroundCalculator()->PreviousRound();
  // GENERAL
  else if (memcmp(name, "session", 7)==0 && strlen(name)==7)	*result = SessionCounter()->session_id();
  else if (memcmp(name, "version", 7)==0 && strlen(name)==7)	*result = VERSION_NUMBER;
  // Handreset
  else if (memcmp(name, "handsplayed", 11)==0 && strlen(name)==11) *result = OpenHoldem()->HandresetDetector()->hands_played();
  else if (memcmp(name, "handsplayed_headsup", 19)==0 && strlen(name)==19)  *result = OpenHoldem()->HandresetDetector()->hands_played_headsup();
  else if (name == kEmptyExpression_False_Zero_WhenOthersFoldForce) { *result = kUndefinedZero; }
  // OH-script-messagebox
  else if (memcmp(name, "msgbox$", 7)==0 && strlen(name)>7) {
    // Don't show name messagebox if in parsing-mode
    // We might however want to show a message if we are not yet connected,
    // e.g. in f$ini_function_pn_startup
    write_log(Preferences()->debug_alltherest(), "[CSymbolEngineVariousDataLookup] location Johnny_8\n");
    if (FormulaParser()->IsParsing()) {
	    *result = 0;
    } else {
	    MessageBox_OH_Script_Messages(name);
	    *result = 0;
    }
  } else if ((memcmp(name, "log$", 4)==0) && (strlen(name)>4)) {
    if (!FormulaParser()->IsParsing()) {
      write_log(Preferences()->debug_auto_trace(), 
        "[CSymbolEngineVariousDataLookup] %s -> 0.000 [just logged]\n", name);
      ///GUI()->WhiteInfoBox()->SetCustomLogMessage(name);
    }
    // True (1) is convenient in sequences of ANDed conditions
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=19421
    *result = true;
  } else if ((memcmp(name, "attached_hwnd", 13)==0) && (strlen(name)==13)) {
    *result = int(TableManagement()->AutoConnector()->attached_hwnd());
  } else if ((memcmp(name, "islobby", 7)==0) && (strlen(name)==7)) {
    *result = BasicScraper()->Tablemap()->islobby();
  }  else if ((memcmp(name, "ispopup", 7) == 0) && (strlen(name) == 7)) {
    *result = BasicScraper()->Tablemap()->ispopup();
  } else if ((memcmp(name, "title$", 6) == 0) && (strlen(name) >= 7)) {
    CString substring = CString(name).Mid(6);
    *result = TableState()->TableTitle()->ContainsSubstring(substring);
  } else if ((memcmp(name, kEmptyExpression_False_Zero_WhenOthersFoldForce, strlen(kEmptyExpression_False_Zero_WhenOthersFoldForce))==0) 
      && (strlen(name)==strlen(kEmptyExpression_False_Zero_WhenOthersFoldForce))) {
    *result = kUndefinedZero;
  } else {
    // Special symbol for empty expressions. Its evaluation adds something 
    // meaningful to the log when the end of an open-ended when-condition 
    // gets reached during evaluation.
    *result = kUndefined;
    return false;
  }
  return true;
}

CString CSymbolEngineVariousDataLookup::SymbolsProvided() {
  // This list includes some prefixes of symbols that can't be verified,
  // e.g. pl_ chair$, ....
  CString list = "pl_ vs$ msgbox$ log$ "
    "betround currentround previousround "
    "session version islobby ispopup"
    "handsplayed handsplayed_headsup ";
  return list;
}



