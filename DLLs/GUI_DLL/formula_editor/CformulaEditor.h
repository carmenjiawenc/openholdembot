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
// Purpose: simple wrapper-interface for the complicated scintilla-editor.
//
//******************************************************************************

#include <afxwin.h>

class CDlgFormulaScintilla;

class CFormulaEditor {
public:
  CFormulaEditor();
  ~CFormulaEditor();
public:
  void CreateEditor();
  void DestroyEditor();
public:
  bool IsCreated();
  void SaveUnAppliedChanges();
private:
  bool HasUnappliedChanges();
private:
  CDlgFormulaScintilla* dialog_formula_scintilla;
};