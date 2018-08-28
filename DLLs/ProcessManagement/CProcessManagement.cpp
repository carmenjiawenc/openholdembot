//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: container-class for the process-management
//
//******************************************************************************

#define PROCESS_MANAGEMEMT_DLL_EXPORTS

#include "CProcessManagement.h"
#include "COpenHoldemStarter.h"
#include "CWatchdog.h"

CProcessManagement::CProcessManagement() {
  _openholdem_starter = new COpenHoldemStarter;
  _watchdog = new CWatchdog;
}

CProcessManagement::~CProcessManagement() {
}
