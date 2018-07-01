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
}

CTableManagement::~CTableManagement() {
}

CTableManagement* table_management = NULL;

CTableManagement *TableManagement() {
  if (table_management == NULL) {
    // Lazy initialization 
    table_management = new CTableManagement;
  }
  return table_management;
}