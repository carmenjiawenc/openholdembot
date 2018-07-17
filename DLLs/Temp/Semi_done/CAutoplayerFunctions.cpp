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

#include "CAutoplayerFunctions.h"
#include "CFunctionCollection.h"
#include <assert.h>
#include "..\Debug_DLL\debug.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\StringFunctions_DLL\string_functions.h"
#include "..\Symbols_DLL\CBetroundCalculator.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineAutoplayer.h"
#include "..\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\Symbols_DLL\CSymbolEngineOpenPPL.h"
#include "..\Symbols_DLL\CSymbolEngineTableLimits.h"
#include "..\Symbols_DLL\CSymbolEngineUserchair.h"

void CAutoplayerFunctions::CalcPrimaryFormulas() {
  write_log(Preferences()->debug_formula(), "[CAutoplayerFunctions] CalcPrimaryFormulas()\n");
  ///assert(EngineContainer()->symbol_engine_autoplayer()->isfinalanswer());

  // Otherwiese: OH-script
  assert(!FunctionCollection()->IsOpenPPLProfile());
  CalcPrimaryFormulasOHScript();
}

void CAutoplayerFunctions::CalcPrimaryFormulasOHScript() {
  write_log(Preferences()->debug_formula(), "[CAutoplayerFunctions] CalcPrimaryFormulasOHScript()\n");
	for (int i=k_autoplayer_function_beep; i<=k_autoplayer_function_fold; i++) {
		double result = FunctionCollection()->Evaluate(k_standard_function_names[i], kAlwaysLogAutoplayerFunctions);
		write_log(Preferences()->debug_formula(), "[CAutoplayerFunctions] Primary formulas; %s: %f\n", 
			k_standard_function_names[i], result);
	}
}

void CAutoplayerFunctions::CalcPrimaryFormulasOpenPPL() {
 write_log(Preferences()->debug_formula(), "[CAutoplayerFunctions] CalcPrimaryFormulasOpenPPL()\n");
  // Now do the main evaluation
  int betround = BetroundCalculator()->betround();
	if (betround < kBetroundPreflop || betround > kBetroundRiver) {
    write_log(Preferences()->debug_formula(), 
      "[CAutoplayerFunctions] Betround out of range. Can't calculate OpenPPL-decision\n");
    return;
  }
  double decision = FunctionCollection()->Evaluate(k_OpenPPL_function_names[betround], 
    kAlwaysLogAutoplayerFunctions);
  write_log(Preferences()->debug_formula(), 
    "[CAutoplayerFunctions] Decision (non-translated) = %.2f\n", decision);
  TranslateOpenPPLDecisionToAutoplayerFunctions(decision);
}

void CAutoplayerFunctions::TranslateOpenPPLDecisionToAutoplayerFunctions(double decision) {
}

void CAutoplayerFunctions::CalculateSingleOpenPPLBackupAction(
    
}

void CAutoplayerFunctions::CalculateOpenPPLBackupActions() {
}

double CAutoplayerFunctions::BetSizeForPercentagedPotsizeBet(double decision) {
  double percentage_0_100 = -100 * decision;
  write_log(Preferences()->debug_formula(), 
    "[CAutoplayerFunctions] Calculate f$betsize for %.2f percent potsize\n",
    percentage_0_100);
  assert(EngineContainer()->symbol_engine_chip_amounts() != NULL);
  assert(EngineContainer()->symbol_engine_userchair() != NULL);
  assert(EngineContainer()->symbol_engine_userchair()->userchair_confirmed());
  double betsize = TableState()->User()->_bet.GetValue() 
    + EngineContainer()->symbol_engine_chip_amounts()->call() 
    + (-1 * decision) * (EngineContainer()->symbol_engine_chip_amounts()->pot() + EngineContainer()->symbol_engine_chip_amounts()->call());
    write_log(Preferences()->debug_formula(), 
      "[CAutoplayerFunctions] f$betsize is %s\n",
    Number2CString(betsize, 2));
  return betsize;
}

void CAutoplayerFunctions::CalcSecondaryFormulas(void) {
  // Not considering k_standard_function_shoot_replay_frame here,
  // as this function gets handled by CSymbolEngineReplayFrameController
	for (int i=k_hopper_function_sitin; i<= k_standard_function_allin_on_betsize_balance_ratio; ++i) {
    double result = FunctionCollection()->Evaluate(k_standard_function_names[i], Preferences()->log_hopper_functions());
		write_log(Preferences()->debug_formula(), "[CAutoplayerFunctions] Secondary formulas; %s: %f\n", 
			  k_standard_function_names[i], result);
	}
}
