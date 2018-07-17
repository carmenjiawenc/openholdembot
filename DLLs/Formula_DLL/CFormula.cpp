//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Container-class for all formula related functionality
//
//******************************************************************************

#define FORMULA_DLL_EXPORTS

#include "CFormula.h"
#include <afxwin.h>
#include "CAutoplayerLogic.h"
#include "CAutoplayerTrace.h"
#include "CFunctionCollection.h"

CFormula::CFormula() {
  _autoplayer_logic = new CAutoplayerLogic;
  _autoplayer_trace = new CAutoplayerTrace;
  _function_collection = new CFunctionCollection;
}

CFormula::~CFormula() {
  /// delete members
}

CFormula* formula = NULL;

CFormula* Formula() {
  if (formula == NULL) {
    // Lazy initialization
    formula = new CFormula;
  }
  return formula;
}