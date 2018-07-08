//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Interface to the user.DLL
//
//*******************************************************************************

#include "CSymbolEngineUserDLL.h"
#include "CEngineContainer.h"
#include "CIteratorThread.h"
#include "CSymbolenginePrWin.h"
#include "CSymbolengineVersus.h"
#include "..\Debug_DLL\debug.h"
#include "..\Formula_DLL\COHScriptList.h"
#include "..\Formula_DLL\CFormulaParser.h"
#include "..\Formula_DLL\CFunctionCollection.h"
#include "..\Globals_DLL\globals.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\TableState_DLL\CTableTitle.h"
#include "..\TableState_DLL\TableState.h"
#include "..\User_DLL\user.h"

CSymbolEngineUserDLL::CSymbolEngineUserDLL() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineUserDLL::~CSymbolEngineUserDLL() {
}

void CSymbolEngineUserDLL::InitOnStartup() {
  DLLUpdateOnNewFormula();
}

void CSymbolEngineUserDLL::UpdateOnConnection() {
  DLLUpdateOnConnection();
}

void CSymbolEngineUserDLL::UpdateOnHandreset() {
  DLLUpdateOnHandreset();
}

void CSymbolEngineUserDLL::UpdateOnNewRound() {
  DLLUpdateOnNewRound();
}

void CSymbolEngineUserDLL::UpdateOnMyTurn() {
  DLLUpdateOnMyTurn();
}

void CSymbolEngineUserDLL::UpdateOnHeartbeat() {
  DLLUpdateOnHeartbeat();
}

bool CSymbolEngineUserDLL::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "dll$", 4) != 0) {
    // Symbol of a different symbol-engine
    return false;
  }
  *result = ProcessQuery(name);
	return true;
}

CString CSymbolEngineUserDLL::SymbolsProvided() {
  return " ";
}

//*******************************************************************************
//
// Exported functions, needed by the DLL
//
//*******************************************************************************

EXE_IMPLEMENTS double GetSymbol(const char* name_of_single_symbol__not_expression) {
  CString	str = "";
  str.Format("%s", name_of_single_symbol__not_expression);
  if (strcmp(str, "cmd$recalc") == 0) {
    // restart iterator thread
    EngineContainer()->symbol_engine_prwin()->IteratorThread()->RestartPrWinComputations();
    // Recompute versus tables
    EngineContainer()->symbol_engine_versus()->GetCounts();
    // Busy waiting until recalculation got finished.
    // Nothing better to do, as we already evaluate bot-logic,
    // so we can't continue with another heartbeat or something else.
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=19012
    while (EngineContainer()->symbol_engine_prwin()->IteratorThread()->IteratorThreadComplete()) {
      Sleep(100);
    }
    return 0;
  }
  double result = kUndefined;
  EngineContainer()->EvaluateSymbol(name_of_single_symbol__not_expression,
    &result,
    kAlwaysLogDLLMessages);
  return result;
}

EXE_IMPLEMENTS void* GetPrw1326() {
  assert(EngineContainer()->symbol_engine_prwin()->IteratorThread() != NULL);
  return (void*)EngineContainer()->symbol_engine_prwin()->IteratorThread()->prw1326();
}

EXE_IMPLEMENTS char* GetHandnumber() {
  return "42";/// OpenHoldem()->HandresetDetector()->GetHandNumber().GetBuffer();
}

EXE_IMPLEMENTS char* GetPlayerName(int chair) {
  if (chair < 0) {
    return nullptr;
  }
  if (chair > kLastChair) {
    return nullptr;
  }
  return TableState()->Player(chair)->name().GetBuffer();
}

EXE_IMPLEMENTS char* GetTableTitle() {
  return TableState()->TableTitle()->Title().GetBuffer();
}

EXE_IMPLEMENTS void ParseHandList(const char* name_of_list, const char* list_body) {
  COHScriptList* p_new_list = new COHScriptList(name_of_list, list_body);
  FormulaParser()->ParseFormula(p_new_list);
  FunctionCollection()->Add(p_new_list);
}

EXE_IMPLEMENTS char* ScrapeTableMapRegion(char* p_region, int& p_returned_lengh) {
  CString result = BasicScraper()->ScrapeRegion(p_region);
  if (result.GetLength() > 0) {
    p_returned_lengh = result.GetLength() + 1;
    char* returnStr = static_cast<char*>(LocalAlloc(LPTR, p_returned_lengh));
    strcat(returnStr, result);
    return returnStr;
  }
  p_returned_lengh = kUndefined;
  return nullptr;
}

// EXE_IMPLEMENTS void SendChatMessage(char *message)
// gets implemented by PokerChat.cpp

EXE_IMPLEMENTS void WriteLog(char* fmt, ...) {
  // Docu about ellipsis and variadic macro:
  // http://msdn.microsoft.com/en-us/library/ms177415(v=vs.80).aspx
  // http://stackoverflow.com/questions/1327854/how-to-convert-a-variable-argument-function-into-a-macro
  va_list args;
  va_start(args, fmt);
  write_log_vl(kAlwaysLogDLLMessages, fmt, args);
  va_end(args);
}