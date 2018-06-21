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
//   
//
//******************************************************************************

#include "CVirtualSymbolEngine.h"

class CSymbolEngineFlags: public CVirtualSymbolEngine {
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
  int GetFlagMax();
  void ClearAllflags();
};



  } else if (memcmp(name, "f", 1) == 0 && strlen(name) == 2) {
    if (p_flags_toolbar != NULL) {
      *result = GUI()->FlagsToolbar()->GetFlag(RightDigitCharacterToNumber(name));
    } else {
      *result = kUndefinedZero;
    }
  } else if (memcmp(name, "f", 1) == 0 && strlen(name) == 3) {
    if (p_flags_toolbar != NULL) {
      *result = GUI()->FlagsToolbar()->GetFlag(10 * RightDigitCharacterToNumber(name, 1) + RightDigitCharacterToNumber(name, 0));
    } else {
      *result = kUndefinedZero;
    }
  } else if (memcmp(name, "fmax", 4) == 0 && strlen(name) == 4) {
    if (p_flags_toolbar != NULL) {
      *result = GUI()->FlagsToolbar()->GetFlagMax();
    } else {
      *result = kUndefinedZero;
    }
  } else if (memcmp(name, "flagbits", 8) == 0 && strlen(name) == 8) {
    if (p_flags_toolbar != NULL) {
      *result = GUI()->FlagsToolbar()->GetFlagBits();
    } else {
      *result = kUndefinedZero;
    }
	
	
	
	list += RangeOfSymbols("f%i", 0, 19);
	    "fmax flagbits "