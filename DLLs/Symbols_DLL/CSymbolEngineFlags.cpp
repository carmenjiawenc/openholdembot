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

#define SYMBOLS_DLL_EXPORTS

#include "CSymbolEngineFlags.h"
#include "..\Numerical_Functions_DLL\NumericalFunctions.h"
#include "..\StringFunctions_DLL\string_functions.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

CSymbolEngineFlags::CSymbolEngineFlags() {
  ClearAllflags();
}

CSymbolEngineFlags::~CSymbolEngineFlags() {
}

void CSymbolEngineFlags::InitOnStartup() {
  ClearAllflags();
}

void CSymbolEngineFlags::UpdateOnConnection() {
  ClearAllflags();
}

void CSymbolEngineFlags::UpdateOnHandreset() {
}

void CSymbolEngineFlags::UpdateOnNewRound() {
}

void CSymbolEngineFlags::UpdateOnMyTurn() {
}

void CSymbolEngineFlags::UpdateOnHeartbeat() {
}

void CSymbolEngineFlags::SetFlag(int index, bool new_value) {
  AssertRange(index, 0, kLastFlag);
  _flags[index] = new_value;
}

bool CSymbolEngineFlags::GetFlag(int index) {
  AssertRange(index, 0, kLastFlag);
  return _flags[index];
}

void CSymbolEngineFlags::ClearAllflags() {
  for (int i = 0; i < kNumberOfFlags; ++i) {
    _flags[i] = false;
  }
}

bool CSymbolEngineFlags::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "f", 1) != 0) {
    return false;
  }
 if (strlen(name) == 2) {
   *result = _flags[RightDigitCharacterToNumber(name)];
 }
 else if (strlen(name) == 3) {
   *result = _flags[10 * RightDigitCharacterToNumber(name, 1) + RightDigitCharacterToNumber(name, 0)];
 } else {
   return false;
 }
 return true;
}

///!!! docu: removal of flagbits, fmax
CString CSymbolEngineFlags::SymbolsProvided() {
  CString list;
  list += RangeOfSymbols("f%i", 0, kLastFlag);
  return list;
}