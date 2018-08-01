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
// Purpose: symbol-table for the parser,
//   remembering function definitions and usages.
//
//******************************************************************************

#include <afxwin.h>
#include <map>
#include "..\MemoryManagement_DLL\CSpaceOptimizedGlobalObject.h"

class CParserSymbolTable: public CSpaceOptimizedGlobalObject {
 public:
  CParserSymbolTable();
  ~CParserSymbolTable();
 public:
  void Clear();
  void VerifySymbol(CString name);
 private:
 private:
  std::map<CString, bool> _known_symbols;
  std::map<CString, bool> _used_functions;
};