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
// Purpose: Container-class for all formula related functionality
//
//******************************************************************************

#include "LibDef.h"

class CAutoplayerTrace;
class CFunctionCollection;

class FORMULA_DLL_API CFormula {
public:
  CFormula();
  ~CFormula();
public:
  CAutoplayerTrace* AutoplayerTrace() {
    return _autoplayer_trace;
  }
  CFunctionCollection* FunctionCollection() {
    return _function_collection;
  }
private:
  CAutoplayerTrace* _autoplayer_trace;
  CFunctionCollection* _function_collection;
};

CFormula* Formula();