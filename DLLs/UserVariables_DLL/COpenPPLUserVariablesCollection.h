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
// Purpose: Data container for OpenPPL user-variables.
//   If they exist they evaluate to true.
//   The collection gets cleared every heartbeat.
//
//******************************************************************************

#include <afxwin.h>
#include <map>
#include "LibDef.h"

class USER_VARIABLES__DLL_API COpenPPLUserVariablesCollection {
 public:
  COpenPPLUserVariablesCollection();
  ~COpenPPLUserVariablesCollection();
 public:
  // Mandatory reset-functions
  void InitOnStartup();
  void UpdateOnConnection();
  void UpdateOnHandreset();
  void UpdateOnNewRound();
  void UpdateOnMyTurn();
  void UpdateOnHeartbeat();
 public:
  void Set(CString symbol);
  bool EvaluateSymbol(const CString name, double *result, bool log = false);
 private:
  std::map<CString, bool> _user_variables;
};

USER_VARIABLES__DLL_API COpenPPLUserVariablesCollection* OpenPPLUserVariablesCollection();