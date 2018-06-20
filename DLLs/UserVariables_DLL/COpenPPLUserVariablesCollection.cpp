//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Data container for OpenPPL user-variables.
//   If they exist they evaluate to true.
//   The collection gets cleared every heartbeat.
//
//******************************************************************************

#define USER_VARIABLES_DLL_EXPORTS

#include "COpenPPLUserVariablesCollection.h"
#include "..\Debug_DLL\debug.h"
#include "..\Globals_DLL\globals.h"
#include "..\Preferences_DLL\Preferences.h"

COpenPPLUserVariablesCollection::COpenPPLUserVariablesCollection() {
}

COpenPPLUserVariablesCollection::~COpenPPLUserVariablesCollection() {
  _user_variables.clear();
}

void COpenPPLUserVariablesCollection::InitOnStartup() {
}

void COpenPPLUserVariablesCollection::UpdateOnConnection() {
}

void COpenPPLUserVariablesCollection::UpdateOnHandreset() {
  // All user-variables are for the current hand only 
  // and get deleted on hand-reset.
  write_log(Preferences()->debug_symbolengine_open_ppl(),
    "[COpenPPLUserVariablesCollection] Deleting all user variables on hand-reset\n");
  _user_variables.clear();
}

void COpenPPLUserVariablesCollection::UpdateOnNewRound() {
}

void COpenPPLUserVariablesCollection::UpdateOnMyTurn() {
}

void COpenPPLUserVariablesCollection::UpdateOnHeartbeat() {
}

void COpenPPLUserVariablesCollection::Set(CString symbol) {
  write_log(Preferences()->debug_symbolengine_open_ppl(),
    "[COpenPPLUserVariablesCollection] Setting user-variable %s\n", symbol);
  _user_variables[symbol] = true;
}

bool COpenPPLUserVariablesCollection::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  if (_memicmp(name, "user", 4) != 0) {
    // Not a user-variable
    return false;
  }
  if (memcmp(name.Mid(4), "chair", 5) == 0) {
    // Symbol "userchair", not a user-variable
    return false;
  }
  // Try to look it up
  if (_user_variables[name]) {
    write_log(Preferences()->debug_symbolengine_open_ppl(),
      "[COpenPPLUserVariablesCollection] user-variable exists: %s\n", name);
    *result = double(true);
    return true;
  }
  write_log(Preferences()->debug_symbolengine_open_ppl(),
    "[COpenPPLUserVariablesCollection] user-variable does not exist: %s\n", name);
  *result = double(false);
  return true;
}

// SymbolsProvided() does not make much sense here
// as we only know the prefixes

COpenPPLUserVariablesCollection* open_PPL_user_variables_collection = NULL;

COpenPPLUserVariablesCollection* OpenPPLUserVariablesCollection() {
  if (open_PPL_user_variables_collection == NULL) {
    // Lazy initialization 
    open_PPL_user_variables_collection = new COpenPPLUserVariablesCollection;
  }
  return open_PPL_user_variables_collection;
}