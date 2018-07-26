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
// Purpose:
//
//******************************************************************************

#include <assert.h>
#include "CVirtualSymbolEngine.h"
#include "CSymbolEngineTableLimits.h"
#include "LibDef.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

class SYMBOLS_DLL_API CSymbolEngineAutoplayer: public CVirtualSymbolEngine {
 public:
	CSymbolEngineAutoplayer();
	~CSymbolEngineAutoplayer();
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
	int  myturnbits()    { return _myturnbits; }
	bool ismyturn()      { return ((_myturnbits & k_my_turn_bits_fold_call_raise) != 0); } //!!! at least two buttons
	bool issittingin()   { return _issittingin; }
	bool issittingout()  { return !issittingin(); }
	bool isautopost()		 { return _isautopost; }
	bool isfinalanswer() { return _isfinalanswer; }
  bool isfinaltable();
 public:
	// Especially needed to start the PrWin-calculations
	bool IsFirstHeartbeatOfMyTurn();
	// Visible buttons
	CString GetFCKRAString();
 private:
	void CalculateMyTurnBits();
	void CalculateSitInState();
	void CalculateFinalAnswer();
 private:
	int  _myturnbits;
	bool _issittingin;
	bool _isautopost;
	bool _isfinalanswer; 
 private:
	bool _last_myturnbits;
};