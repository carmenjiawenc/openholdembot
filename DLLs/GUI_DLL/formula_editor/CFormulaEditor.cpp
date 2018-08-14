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

#include "CFormulaEditor.h"
#include <assert.h>
#include "DialogFormulaScintilla.h"
#include "..\CGUI.h"
#include "..\Toolbar\CFlagsToolbar.h"
#include "..\..\WindowFunctions_DLL\window_functions.h"

extern bool Scintilla_RegisterClasses(void *hInstance);
extern bool Scintilla_ReleaseResources();

CFormulaEditor::CFormulaEditor() {
  dialog_formula_scintilla = NULL;
}

CFormulaEditor::~CFormulaEditor() {
}

void CFormulaEditor::CreateEditor() {
  if (IsCreated()) {
    return;
  }
  if (GUI()->FlagsToolbar()->IsAutoplayerButtonChecked()) {
    // The menu item Edit->Formula is disabled,
    // this is just an extra failsafe.
    return;
  }
  Scintilla_RegisterClasses(AfxGetInstanceHandle());
  // Initialize richedit2 library
  AfxInitRichEdit2();
  assert(dialog_formula_scintilla == NULL);
  dialog_formula_scintilla = new CDlgFormulaScintilla();///this);
  dialog_formula_scintilla->Create(CDlgFormulaScintilla::IDD);///, this);
  dialog_formula_scintilla->ShowWindow(SW_SHOW);
}
 
void CFormulaEditor::DestroyEditor() {
  if (!IsCreated()) {
    return;
  }
  ///
  dialog_formula_scintilla->DestroyWindow();
  dialog_formula_scintilla = NULL;
  Scintilla_ReleaseResources();
}

bool CFormulaEditor::IsCreated() {
  return (dialog_formula_scintilla != NULL);
}

bool CFormulaEditor::HasUnappliedChanges() {
  if (!IsCreated()) {
    return false;
  }
  return (dialog_formula_scintilla->m_dirty);
}

void CFormulaEditor::SaveUnAppliedChanges() {
  if(!HasUnappliedChanges()) {
    return;
  }
  if (MessageBox_Interactive(
      "The Formula Editor has un-applied changes.\n"
      "Really exit?",
      "Formula Editor", MB_ICONWARNING | MB_YESNO) == IDNO) {
    return;
  }
  ///
}


/*#
if (GUI()->DlgFormulaScintilla()) {
  
  BOOL	bWasShown = ::IsWindow(GUI()->DlgFormulaScintilla()->m_hWnd) && GUI()->DlgFormulaScintilla()->IsWindowVisible();
  GUI()->DlgFormulaScintilla()->DestroyWindow();
  if (bWasShown) {
    return;
  }
}
*/