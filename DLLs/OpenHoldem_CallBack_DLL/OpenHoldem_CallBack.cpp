//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Managing access to global data,
//   avoiding circular dependency of DLLs to OpenHoldem.
//   Temp solution until we get rid of all global data,
//   once mpdulatization has been improved
//
//******************************************************************************

#define OPENHOLDEM_CALLBACK_DLL_EXPORTS

#include "OpenHoldem_CallBack.h"
#include "..\GUI_DLL\CGUI.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\TableState_DLL\TableState.h"

double EvaluateSymbol(CString symbol) { 
  return 3;  ///!!!
}

int NumberOfCardsPerPlayer() {
  return 3; ///!!!
}