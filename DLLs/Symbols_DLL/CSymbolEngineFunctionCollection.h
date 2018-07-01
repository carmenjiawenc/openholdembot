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
// Purpose:  Providing a list of build-in higher-level OpenPPL-symbols
//   and user-defined functions and lists. 
//
//   * CFunctionCollection (Formula.DLL) provides the actual functionality
//   * CSymbolEngineFunctionCollection (Symbols.DLL) provides the integration 
//     into the symbol-engine
//
//******************************************************************************

#define _ALLOW_KEYWORD_MACROS
#include "CVirtualSymbolEngine.h"

// We make the wrapper of the collection a symbol-engine because of 
// * reset-functions 
// * Evaluate()-function,
// * SymbolsProvided() for syntax-highlighting
// The actual function-collection is part of the formula.DLL
// for technical reasons.
class CSymbolEngineFunctionCollection: public CVirtualSymbolEngine {
 public:
  CSymbolEngineFunctionCollection();
  virtual ~CSymbolEngineFunctionCollection();
 public:
  bool EvaluateSymbol(const CString name, double *result, bool log = false);
  double Evaluate(CString function_name, bool log = false);
 public: 
  CString SymbolsProvided();
 public:
  // Mandatory reset-functions
  void InitOnStartup();
  void UpdateOnConnection();
  void UpdateOnHandreset();
  void UpdateOnNewRound();
  void UpdateOnMyTurn();
  void UpdateOnHeartbeat();
};