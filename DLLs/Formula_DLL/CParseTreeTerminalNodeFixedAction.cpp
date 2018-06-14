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

 
#include "CParseTreeTerminalNodeFixedAction.h"
#include <math.h>
///#include "CAutoplayerTrace.h"

///#include "CMemoryPool.h"
#include "CFunctionCollection.h"
#include "CParserSymbolTable.h"
#include "TokenizerConstants.h"
#include "..\Debug_DLL\debug.h"
#include "..\Numerical_Functions_DLL\Numerical_Functions.h"
#include "..\StringFunctions_DLL\string_functions.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\Symbols_DLL\CSymbolEngineMemorySymbols.h"
#include "..\Symbols_DLL\CSymbolEngineOpenPPLUserVariables.h"
#include "..\Symbols_DLL\CSymbolEngineTableLimits.h"
#include "..\WindowFunctions_DLL\window_functions.h"

CParseTreeTerminalNodeFixedAction::CParseTreeTerminalNodeFixedAction(
  int relative_line_number, CString name) : 
  CParseTreeTerminalNodeIdentifier(relative_line_number, 
    name) {
}

CParseTreeTerminalNodeFixedAction::~CParseTreeTerminalNodeFixedAction() {
}

CString CParseTreeTerminalNodeFixedAction::EvaluateToString(bool log /* = false */) {
  double numerical_result = Evaluate(log);
  CString result;
  if (IsInteger(numerical_result) && EvaluatesToBinaryNumber()) {
    // Generqate binary representation;
    result.Format("%s", IntToBinaryString(int(numerical_result)));
  } else {
    // Generate floating-point representation
    // with 3 digits precision
    result.Format("%.3f", numerical_result);
  }
  return result;
}

CString CParseTreeTerminalNodeFixedAction::Serialize() {
  if (_node_type == kTokenIdentifier) {
    return _terminal_name;
  } else {
    // Unhandled note-type, probably new and therefore not yet handled
   write_log(k_always_log_errors, "[CParseTreeTerminalNode] ERROR: Unhandled node-type %i in serialization of parse-tree\n",
      _node_type);
    return "";
  }
}

