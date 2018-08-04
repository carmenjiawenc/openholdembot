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

#define TABLE_MANAGEMENT_DLL_EXPORTS

#include "CTableManagement.h"

CTableManagement::CTableManagement() {
  _shared_mem = NULL;
  _auto_connector = NULL;
  _popup_handler = NULL;
  _table_positioner = NULL;
}

CTableManagement::~CTableManagement() {
  ///!!! clean up
}

void CTableManagement::Initialize() {
  _shared_mem = new CSharedMem;
  _auto_connector = new CAutoConnector;
  _popup_handler = new CPopupHandler;
  _table_positioner = new CTablePositioner;
}

CTableManagement* table_management = NULL;

CTableManagement *TableManagement() {
  if (table_management == NULL) {
    // Lazy initialization 
    table_management = new CTableManagement;
    table_management->Initialize();
  }
  return table_management;
}