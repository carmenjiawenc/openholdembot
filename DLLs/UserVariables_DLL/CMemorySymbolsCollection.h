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
// Purpose: storing/recalling OH-script memory-symbols
//   and OpenPPL user-variables
//
//******************************************************************************

#include <afxwin.h>
#include <map>
#include "LibDef.h"

USER_VARIABLES__DLL_API class CMemorySymbolsCollection {
 public:
  CMemorySymbolsCollection();
  ~CMemorySymbolsCollection();
 public:
  // Mandatory reset-functions
  void UpdateOnConnection();
 public:
  bool EvaluateSymbol(const CString name, double *result, bool log = false);
 private:
  void Store(CString command);
  void Increment(CString command);
  void Add(CString command);
  void Sub(CString command);
  double Recall(CString command);
 private:
  CString RightHandSide(CString command);
  CString LeftHandSide(CString command);
  double EvaluateRightHandExpression(CString right_hand_value);
 private:
  void ErrorInvalidMemoryStoreCommand(CString command);
  void ErrorUnnamedMemorySymbol(CString command);
 private:
  // The keys for storage and lookup are lower-cases
  // To make memory-symbols case insensitive, 
  // i.e. less error-prone as there are no compile-time-checks.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19224
  CString LowerCaseKey(CString symbol);
 private:
  std::map<CString, double> _memory_symbols;
};

USER_VARIABLES__DLL_API CMemorySymbolsCollection* MemorySymbolsCollection();