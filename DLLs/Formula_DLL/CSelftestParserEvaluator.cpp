//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: 
//
//******************************************************************************

#include "CSelftestParserEvaluator.h"
#include "CFormula.h"
#include "CFunctionCollection.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\WindowFunctions_DLL\window_functions.h"

CSelftestParserEvaluator::CSelftestParserEvaluator() {
}

CSelftestParserEvaluator::~CSelftestParserEvaluator() {
}

void CSelftestParserEvaluator::Test() {
  if (!Formula()->FunctionCollection()->Exists(kSelftestName)) {
    MessageBox_Error_Warning("Can't find selftest-function\n"
      "Please get in contact with the developmen team.\n");
    return;
  }
  double result = Formula()->FunctionCollection()->Evaluate(kSelftestName);
  if (result != kSelftestExpectedResult) {
    CString message;
    message.Format("Selftest failed.\n"
      "Please get in contact with the developmen team.\n"
      "Calculated result = %.3f\n"
      "Expected result = %.3f\n",
      result, kSelftestExpectedResult);
    MessageBox_Error_Warning(message);
  }
}