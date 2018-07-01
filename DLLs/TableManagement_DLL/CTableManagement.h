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
// Purpose: container-class for table-related functionality
//   like auto-connector and popup-hanlder.
//
//******************************************************************************

#include "LibDef.h"

#include "CAutoConnector.h"
#include "CPopupHandler.h"
#include "CSharedMem.h"
#include "CTablePositioner.h"

class TABLE_MANAGEMENT_DLL_API CTableManagement {
public:
  CTableManagement();
  ~CTableManagement();
public:
  CAutoConnector *AutoConnector() {
     return &_auto_connector;
  }
  CPopupHandler *PopupHandler() {
    return &_popup_handler;
  }
  CSharedMem *SharedMem() {
    return &_shared_mem;
  }
  CTablePositioner *TablePositioner() {
    return &_table_positioner;
  }
private:
  // CSharedMem shared_mem first, because order of initialization matters.
  // The auto-connector needs it.
  CSharedMem _shared_mem;
  CAutoConnector _auto_connector;
  CPopupHandler _popup_handler;
  CTablePositioner _table_positioner;
};

TABLE_MANAGEMENT_DLL_API CTableManagement *TableManagement();