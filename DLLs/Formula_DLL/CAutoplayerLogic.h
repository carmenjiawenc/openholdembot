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
// Purpose: evaluating autoplayer-functions
//   and calculating OpenPPL backup-actions.
//
//******************************************************************************

#include "LibDef.h"

class FORMULA_DLL_API CAutoplayerLogic {
  friend class CAutoplayer;
  friend class CAutoplayerTrace;
public:
  CAutoplayerLogic();
  ~CAutoplayerLogic();
public:
  void EvaluateOpenPPL();
  ///double EvaluateAutoplayerFunction(const int function_code);
public:
  double GetValue(const int function_code);
  void SetValue(const int function_code, double new_value);
public:
  void CalcPrimaryFormulas();
  void CalcSecondaryFormulas();
private:
  void CalcPrimaryFormulasOpenPPL();
  void TranslateOpenPPLDecisionToAutoplayerFunctions(double decision);
  void CalculateOpenPPLBackupActions();
  void CalculateSingleOpenPPLBackupAction(
    int potential_action, int potential_backup);
  void CheckIfDecisionMatchesElementaryAction(int decision, int action);
private:
  // Aggressive backup-actions
  bool IsFoldAllinSituation(); // To do: FCA
private:
  bool IsPercentagePotsizeBet(double decision);
};