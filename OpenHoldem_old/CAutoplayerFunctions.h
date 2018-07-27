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

///#include "CSpaceOptimizedGlobalObject.h"

class CAutoplayerFunctions /*#: public CSpaceOptimizedGlobalObject */{
 public:
	CAutoplayerFunctions();
 public:
	void CalcPrimaryFormulas();
	void CalcSecondaryFormulas();
	double GetAutoplayerFunctionValue(const int function_code);
 private:
  void CalcPrimaryFormulasOHScript();
  
  bool IsPercentagePotsizeBet(double decision);
  double BetSizeForPercentagedPotsizeBet(double decision); //!!!!! dupliate to betpot-calculations?
 private:
  void CheckIfDecisionMatchesElementaryAction(int decision, int action);
 private:
	CCritSec	m_critsec;
};