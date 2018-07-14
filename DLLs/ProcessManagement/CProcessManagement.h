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
// Purpose: container-class for the process-management
//
//******************************************************************************

class COpenHoldemStarter;
class CWatchdog;

class CProcessManagement {
public:
  CProcessManagement();
  ~CProcessManagement();
public:
  COpenHoldemStarter* OpenHoldemStarter();
  CWatchdog* Watchdog();
};