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
// Purpose: time symbols for OH-script and OpenPPL
//
//******************************************************************************

#include "CVirtualSymbolEngine.h"
#include "LibDef.h"

class SYMBOLS_DLL_API CSymbolEngineTime: public CVirtualSymbolEngine {
 public:
  CSymbolEngineTime();
  ~CSymbolEngineTime();
 public:
  // Mandatory reset-functions
  void InitOnStartup();
  void UpdateOnConnection();
  void UpdateOnHandreset();
  void UpdateOnNewRound();
  void UpdateOnMyTurn();
  void UpdateOnHeartbeat();
 public:
  void UpdateOnAutoPlayerAction();
 public:
  // Public accessors
  bool EvaluateSymbol(const CString name, double *result, bool log = false);
  CString SymbolsProvided();
 public:
  double elapsed(); 
  double elapsedhand(); 
  double elapsedauto(); 
  double elapsedtoday(); 
 public:
  // for the f$delay-function
  double elapsedmyturn();
 private:
  time_t _elapsedautohold;	 // The last time autoplayer acted
  time_t _elapsedhold;		   // The time we "sat down"
  time_t _elapsedhandhold;	 // The time since start of last hand
  time_t _elapsedmyturnhold;// The time when my turn started
 private:
  bool _last_heartbeat_was_my_turn;
};