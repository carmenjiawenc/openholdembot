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
// Purpose: Detecting if we play a holdem or omaha,
//   needed e.g. for automatic adaption of PT-queries.
//
//******************************************************************************

#include "afxwin.h"
#include "CVirtualSymbolEngine.h"
#include "LibDef.h"

class SYMBOLS_DLL_API CSymbolEngineIsOmaha: public CVirtualSymbolEngine {
public:
	CSymbolEngineIsOmaha();
	~CSymbolEngineIsOmaha();
public:
  bool EvaluateSymbol(const CString name, double *result, bool log = false);
  CString SymbolsProvided();
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
  bool isomaha() { return  _isomaha; }
public:
  // The number of cards per player depends on the game-type.
  // This affects cards to be scraped and evaluated.
  // The data containers must be large enough to store kMaxNumberOfCardsPerPlayer.
  int NumberOfCardsPerPlayer();
private:
  bool _isomaha;
};