//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:  Providing a list of build-in higher-level OpenPPL-symbols
//   and user-defined functions and lists. 
//
//   * CFunctionCollection (Formula.DLL) provides the actual functionality
//   * CSymbolEngineFunctionCollection (Symbols.DLL) provides the integration 
//     into the symbol-engine
//
//******************************************************************************

#include "CSymbolEngineFunctionCollection.h"
#include <afxwin.h>
#include "..\Debug_DLL\debug.h"
#include "..\Formula_DLL\CFormula.h"
#include "..\Formula_DLL\CFunctionCollection.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\WindowFunctions_DLL\window_functions.h"

CSymbolEngineFunctionCollection::CSymbolEngineFunctionCollection(){
}

CSymbolEngineFunctionCollection::~CSymbolEngineFunctionCollection() {
}

double CSymbolEngineFunctionCollection::Evaluate(CString function_name, bool log /* = false */) {
  double result = kUndefined;
  Formula()->FunctionCollection()->EvaluateSymbol(function_name,
    &result, log);
  return result;
}

void CSymbolEngineFunctionCollection::UpdateOnConnection() {
  Formula()->FunctionCollection()->ClearCache();
}

void CSymbolEngineFunctionCollection::UpdateOnHandreset() {
}

void CSymbolEngineFunctionCollection::UpdateOnNewRound() {
}

void CSymbolEngineFunctionCollection::UpdateOnMyTurn() {
}

void CSymbolEngineFunctionCollection::UpdateOnHeartbeat() {
  Formula()->FunctionCollection()->ClearCache();
}

bool CSymbolEngineFunctionCollection::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  bool success = Formula()->FunctionCollection()->EvaluateSymbol(
    name, result, log);
  return success;
}

CString CSymbolEngineFunctionCollection::SymbolsProvided() {
  return Formula()->FunctionCollection()->SymbolsProvided();
}
