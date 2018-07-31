//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: interface to the flags toolbar with buttons 0..19
//   The flags-toolbar provides an easy method to configure the
//   botlogic via the OpenHoldem GUI during development..
//
//******************************************************************************

#include <afxcmn.h>
#include <afxwin.h>
#include "CFlagsToolbar.h"
#include "MyCToolBar.h"
#include "..\resource.h"
#include "..\..\Symbols_DLL\CEngineContainer.h"
#include "..\..\Symbols_DLL\CSymbolEngineFlags.h"

#define ENT CSLock lock(m_critsec);

BEGIN_MESSAGE_MAP(CFlagsToolbar, CWnd)
END_MESSAGE_MAP()

CFlagsToolbar::CFlagsToolbar(CFrameWnd *parent_window) {
	_parent_window = parent_window;
	CreateMainToolbar();
	CreateFlagsToolbar();
	AlignToolbars();
  bool to_be_enabled_or_not = false;/// p_autoplayer->autoplayer_engaged();
	m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, to_be_enabled_or_not);
  ResetButtonsOnDisconnect();
  ResetButtonsOnAutoplayerOff();
}

CFlagsToolbar::~CFlagsToolbar() {
}

void CFlagsToolbar::ResetButtonsOnConnect() {
	ResetButtonsOnAutoplayerOff();
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);
  m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, true);
}

void CFlagsToolbar::ResetButtonsOnDisconnect() {
  ResetButtonsOnAutoplayerOff();
	m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
  m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, false);
}

void CFlagsToolbar::ResetButtonsOnAutoplayerOn() {
  m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, false);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, false);
  m_MainToolBar.GetToolBarCtrl().EnableButton(ID_EDIT_FORMULA, false);
  CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
}

void CFlagsToolbar::ResetButtonsOnAutoplayerOff() {
  m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, true);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, true);
  m_MainToolBar.GetToolBarCtrl().EnableButton(ID_EDIT_FORMULA, true);
  CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
}

void CFlagsToolbar::OnClickedFlags() {
  assert(ID_NUMBER0 + kLastFlag == ID_NUMBER19);
  for (int i = ID_NUMBER0; i <= ID_NUMBER19; ++i) {
    int button_ID = ID_NUMBER0 + i;
    bool button_state = _tool_bar.GetToolBarCtrl().IsButtonChecked(button_ID);
    EngineContainer()->symbol_engine_flags()->SetFlag(i, button_state);
  }
	// No longer calling EngineContainer()->EvaluateAll();
  // It might cause unexpected side-effects,
  // if we evaluate multiple times during one heartbeat.
  // The effect of this button-click will have to wait
  // till the next heartbeat starts.
}

void CFlagsToolbar::CreateMainToolbar(void) {
	TBBUTTONINFO	tbi;
	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE;
	tbi.fsStyle = TBSTYLE_CHECK;
	// Main toolbar
	m_MainToolBar.CreateEx(_parent_window, NULL, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_MainToolBar.LoadToolBar(IDR_MAINFRAME);
	m_MainToolBar.EnableDocking(CBRS_ALIGN_TOP);
	// Make formula button sticky
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_FORMULA, &tbi);
	// Make validator button sticky
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_VALIDATOR, &tbi);
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_TAGLOGFILE, &tbi);
	// Make scraper output button sticky
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, &tbi);
	// Make autoplayer button sticky, and start it out disabled
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_AUTOPLAYER, &tbi);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
	// Start shoot replay frame button disabled
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, false);
  // Title of floating main toolbar
	m_MainToolBar.SetWindowText("Main");
}

void CFlagsToolbar::CreateFlagsToolbar(void) {
	TBBUTTONINFO	tbi;
	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE;
	tbi.fsStyle = TBSTYLE_CHECK;
	// Flags toolbar
	_tool_bar.CreateEx(_parent_window, NULL, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
   	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	_tool_bar.LoadToolBar(IDR_FLAGS);
	assert((void*)_tool_bar != NULL);
	// Make flags toolbar buttons sticky
  assert(ID_NUMBER0 + kLastFlag == ID_NUMBER19);
  for (int i = ID_NUMBER0; i <= ID_NUMBER19; ++i) {
    _tool_bar.GetToolBarCtrl().SetButtonInfo(i, &tbi);
  }
	_tool_bar.EnableDocking(CBRS_ALIGN_TOP);
	// Title of floating flags toolbar
	_tool_bar.SetWindowText("Flags");
	assert((void*)_tool_bar != NULL);
}

void CFlagsToolbar::AlignToolbars(void) {
	// Put the main toolbar and flags toolbar on the same line
	CRect rectBar1, rectBar2;
	_parent_window->RecalcLayout(true);
	m_MainToolBar.GetWindowRect(rectBar1);
	_tool_bar.GetWindowRect(rectBar2);
	UINT uiBarWidth = rectBar2.Width();
	rectBar2.left = rectBar1.right;
	rectBar2.top = rectBar1.top;
	rectBar2.bottom = rectBar1.bottom;
	rectBar2.right = rectBar1.right + uiBarWidth;
	_parent_window->RecalcLayout();
}

void CFlagsToolbar::UnattachOHFromPokerWindow() {
}

void CFlagsToolbar::EnableButton(int button_ID, bool new_status) {
	m_MainToolBar.GetToolBarCtrl().EnableButton(button_ID, new_status);
}

void CFlagsToolbar::CheckButton(int button_ID, bool new_status) {
	m_MainToolBar.GetToolBarCtrl().CheckButton(button_ID, new_status);
}

bool CFlagsToolbar::IsButtonChecked(int button_ID) {
	return m_MainToolBar.GetToolBarCtrl().IsButtonChecked(button_ID);
}

bool CFlagsToolbar::IsButtonEnabled(int button_ID) {
	return m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(button_ID);
}

/// To do: update on heartbeat