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
// Purpose: flag-symbols for OpenHoldem,
//   representing buttons of the flags-toolbar
//
//******************************************************************************

#include "CVirtualSymbolEngine.h"
#include "LibDef.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

class SYMBOLS_DLL_API CSymbolEngineFlags: public CVirtualSymbolEngine {
 public:
  CSymbolEngineFlags();
  ~CSymbolEngineFlags();
 public:
  // Mandatory reset-functions
  void InitOnStartup();
  void UpdateOnConnection();
  void UpdateOnHandreset();
  void UpdateOnNewRound();
  void UpdateOnMyTurn();
  void UpdateOnHeartbeat();
 public:
  // Public accessors
  bool EvaluateSymbol(const CString name, double *result, bool log = false);
  CString SymbolsProvided();
 public:
  void SetFlag(int index, bool new_value);
  bool GetFlag(int index);
  void ClearAllflags();
 private:
  bool _flags[kNumberOfFlags];
};	