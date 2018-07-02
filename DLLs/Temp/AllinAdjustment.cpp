//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: adjusting betsizes to alin if they are nearly our complete balance,
//  depending on f$allin_on_betsize_balance_ratio
//
//******************************************************************************

#include <assert.h>
#include "AllinAdjustment.h"
///#include "BetpotCalculations.h"
#include "SwagAdjustment.h"
#include "..\Debug_DLL\debug.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\Symbols_DLL\CSymbolEngineGameType.h"
#include "..\Symbols_DLL\CSymbolEngineIsTournament.h"
#include "..\Symbols_DLL\CSymbolEngineUserchair.h"
#include "..\Symbols_DLL\CSymbolEngineTableLimits.h"
#include "..\Tablestate_DLL\CPlayer.h"
#include "..\Tablestate_DLL\TableState.h"

bool ChangeBetsizeToAllin(double amount_to_raise_to) {
  assert(amount_to_raise_to >= 0.0);
  assert(p_function_collection != NULL);
  double critical_betsize_to_balance_ratio =
    0.3;///FunctionCollection()->EvaluateAutoplayerFunction(k_standard_function_allin_on_betsize_balance_ratio);
  // Enabled?
  if (critical_betsize_to_balance_ratio <= 0.0) {
    write_log(Preferences()->debug_allin_adjustment(),
      "[AllinAdjustment] Disabled (<= 0.00)\n");
    return false;
  }
  // Safety measure: good balance?
  if (TableState()->User()->_balance.GetValue() <= 0.0) {
    write_log(Preferences()->debug_allin_adjustment(),
      "[AllinAdjustment] Cancelled due to bad users balance\n");
    return false;
  }
  double total_balance = MaximumPossibleBetsizeBecauseOfBalance();
  double critical_betsize = critical_betsize_to_balance_ratio * total_balance;
  // Safety measure: good old betsize to balance ratio?
  // If our currentbet is "too large" or our balance "too low"
  // then we alreadz should be allin or something is wrong.
  // Act conservatively here
  if (TableState()->User()->_bet.GetValue() >= critical_betsize) {
    write_log(Preferences()->debug_allin_adjustment(),
      "[AllinAdjustment] Cancelled as we already should be allin if input was right.\n");
    return false;
  }
  if (amount_to_raise_to >= critical_betsize) {
    write_log(Preferences()->debug_allin_adjustment(),
      "[AllinAdjustment] Changing betsize %.2f to allin\n", amount_to_raise_to);
    return true;
  } else {
    write_log(Preferences()->debug_allin_adjustment(),
      "[AllinAdjustment] Not changing betsize %.2f to allin\n", amount_to_raise_to);
    return false;
  }
}

bool ChangeBetPotActionToAllin(int betpot_action_code) {
  assert(betpot_action_code >= k_autoplayer_function_betpot_2_1);
  assert(betpot_action_code <= k_autoplayer_function_betpot_1_4);
  return false;/// ChangeBetsizeToAllin(BetsizeForBetpot(betpot_action_code));
}