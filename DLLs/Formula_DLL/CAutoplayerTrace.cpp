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

#define FORMULA_DLL_EXPORTS

#include "CAutoplayerTrace.h"
#include "CFormula.h"
#include "CFunctionCollection.h"
///#include "CHandresetDetector.h"
#include "COHScriptObject.h"
///#include "CopenHoldemStatusbar.h"
#include "..\Debug_DLL\debug.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Tablestate_DLL\TableState.h"

#define ENT CSLock lock(m_critsec);

CAutoplayerTrace::CAutoplayerTrace() {
  Clear();
}

CAutoplayerTrace::~CAutoplayerTrace() {
  Clear();
}

void CAutoplayerTrace::Clear() {
  ENT 
  _indentation = 0;
  _number_of_log_lines = 0;
  _symboltrace_collection.RemoveAll();
  _already_logged_symbols.clear();
}

bool CAutoplayerTrace::SymbolNeedsToBeLogged(CString name) {
  // DLL, as there is no caching and value might change
  if (memcmp(name, "dll$", 4) == 0) return true;
  // Memory-store and recall-commands
  if (memcmp(name, "me_", 3) == 0) return true;
  // OpenPPL-user-variables might also change (once)
  // We don't care about multiple loggings of userchair here
  if (_memicmp(name, "user", 4) == 0) return true;
  // True random numbers that don't get cached,
  // i.e. OH-script "random" and OpenPPL "Random"
  if (memcmp(name, "random", 6) == 0) return true;
  if (memcmp(name, "Random", 6) == 0) return true;
  // Values that already got logged can be ignored
  if (_already_logged_symbols[name] == true) return false;
  // Everything else needs to be logged
  return true;
}

int CAutoplayerTrace::Add(CString symbol) {
  ENT
  write_log(Preferences()->debug_auto_trace(),
    "[CAutoplayerTrace] Add (%s, ...)\n", symbol);
  // This function for symbols without value is for functions only.
  // These functions are eitherpredefined (f$), userdefined (f$)
  // or OpenPPL (upper-cases).
  // The value will be backpatched later.
  assert(COHScriptObject::IsFunction(symbol)
    || COHScriptObject::IsOpenPPLSymbol(symbol));
  CString new_message;
  new_message.Format("%s%s = ",
    Indentation(), symbol);
  _symboltrace_collection.Add(new_message);
  // Nothing to do for _already_logged_symbols here,
  // as this Add()-function is for the first-time-evaluation
  // of f$functions.
  _number_of_log_lines++;
  return (_number_of_log_lines - 1); 
}

void CAutoplayerTrace::Add(CString symbol, double value, bool undefined /* = false */) {
  ENT
  write_log(Preferences()->debug_auto_trace(),
    "[CAutoplayerTrace] Add (%s, %.3f)\n",
    symbol, value);
  if (!SymbolNeedsToBeLogged(symbol)) return;
  CString new_message;
  if (undefined) {
    // For empty functions with NULL parse-tree
    assert(value == kUndefinedZero);
    new_message.Format("%s%s = %.3f [undefined]",
      Indentation(), symbol, value);
  } else if (COHScriptObject::IsFunction(symbol)
      || COHScriptObject::IsOpenPPLSymbol(symbol)) {
    // Function with known value a priori
    new_message.Format("%s%s = %.3f [cached]",
      Indentation(), symbol, value);
  } else {
    // "Normal" symbol
    new_message.Format("%s%s = %.3f",
      Indentation(), symbol, value); 
  }
  _symboltrace_collection.Add(new_message);
  _already_logged_symbols[symbol] = true;
  _number_of_log_lines++;
}

void CAutoplayerTrace::BackPatchValueAndLine(
    int index, double value, int starting_line_of_function) {
  assert(index >= 0);
  assert(index < _number_of_log_lines);
  // Starting line should be > 0, but auto-generated missing 
  // vital functions like f$check can have line == 0.
  assert(starting_line_of_function >= 0);
  int executed_absolute_line = starting_line_of_function 
    + _last_evaluated_relative_line_number;
  // Already done:
  // Indentation, symbol, " = "
  CString complete_message;
  complete_message.Format("%s%.3f [Line %d/%d]", 
    _symboltrace_collection.GetAt(index),
    value,
    _last_evaluated_relative_line_number,
    executed_absolute_line);
  _symboltrace_collection.SetAt(index, complete_message);
}

void CAutoplayerTrace::Indent(bool more) {
  if (more) {
    _indentation += 2;
  } else {
    _indentation -= 2;
  }
  if (_indentation < 0) {
    // This could happen if we calculate and log prwin functions
    // and the heartbeat-threat resets the autoplayer-trace inbetween.
    // Most easy way to fix it: continue gracefully
    _indentation = 0;
  }
}

CString CAutoplayerTrace::Indentation() {
  assert(_indentation >= 0);
  CString format;
  format.Format("%%%ds", _indentation);
  CString indentation;
  indentation.Format(format, "");
  return indentation;
}

void CAutoplayerTrace::Print(const char *action_taken, bool full_log_for_primary_formulas) {
  // Probably not necessary: CSLock lock(log_critsec);
  // as nothing else should happen when the autoplayer is finished
  if (full_log_for_primary_formulas) {
    ///LogPlayers();
    // This information is only meaningful for playing decision f$all .. f$fold
    ///LogBasicInfo(action_taken);
  } else {
    LogSecondaryAction(action_taken);
  }
  LogAutoPlayerTrace();
  Clear();
}

CString CAutoplayerTrace::BestAction() {
  for (int i=k_autoplayer_function_allin; i<=k_autoplayer_function_fold; ++i) {
    if (1/*#Formula()->FunctionCollection()->AutoplayerLogic()->GetAutoplayerFunctionValue(i)*/) {
      if (i == k_autoplayer_function_betsize) {
        // Special treatment for f$betsize
        // e.g. "f$betsize = 201.47"
        CString best_action;
        best_action.Format("%s = %.2f", k_standard_function_names[i],
          Formula()->FunctionCollection()->EvaluateAutoplayerFunction(k_autoplayer_function_betsize));
        return best_action;
      }
      else {
        return k_standard_function_names[i];
      }
    }
  }
  // No action can happen if it is not our turn (best action in GUI)
  return "no action";
}

void CAutoplayerTrace::LogSecondaryAction(const char *action_taken) {
  write_log_separator(true, "Secondary Action");
  write_log(k_always_log_basic_information, "  Action taken:  %s\n", action_taken);
  write_log_separator(true, "");
}

void CAutoplayerTrace::LogAutoPlayerTrace() {
  if (_symboltrace_collection.GetSize() <= 0) {
    return;
  }
  write_log_separator(true, "Autoplayer Trace");
  for (int i=0; i<_symboltrace_collection.GetSize(); ++i) {
	  write_log_nostamp(true, "%s\n", _symboltrace_collection.GetAt(i));
  }
  write_log_separator(true, "");
}


