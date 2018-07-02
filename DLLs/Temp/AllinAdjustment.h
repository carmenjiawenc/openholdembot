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
// Purpose: adjusting betsizes to alin if they are nearly our complete balance,
//  depending on f$allin_on_betsize_balance_ratio
//
//******************************************************************************

bool ChangeBetsizeToAllin(double amount_to_raise_to);
bool ChangeBetPotActionToAllin(int betpot_action_code);
