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
// Purpose: container-class for GUI elements
//
//******************************************************************************

class CDlgFormulaScintilla;
class CDlgScraperOutput;
class CFlagsToolbar;
class CMainFrame;
class COpenHoldemStatusbar;
class COpenHoldemTitle;
class CWhiteInfoBox;

class CGUI {
public:
  CGUI();
  ~CGUI();
public:
  void Update();
public:
  CFlagsToolbar *FlagsToolbar();
  CMainFrame *MainFrame();
  COpenHoldemStatusbar* OpenHoldemStatusbar();
  COpenHoldemTitle* OpenHoldemTitle();
  CDlgFormulaScintilla* DlgFormulaScintilla();
  CDlgScraperOutput *DlgScraperOutput();
  CWhiteInfoBox* WhiteInfoBox();
private:
  void UpdateOnConnection();
  void UpdateOnDisconnection();
};

CGUI* GUI();




/*#if (p_flags_toolbar != NULL) {
delete(p_flags_toolbar);
}
if (p_openholdem_statusbar != NULL) {
delete p_openholdem_statusbar;
}


// Tool bar
//guip_flags_toolbar = new CFlagsToolbar(this);
// Status bar
///p_openholdem_statusbar = new COpenHoldemStatusbar(this);
// Start timer that checks if we should enable buttons
SetTimer(ENABLE_BUTTONS_TIMER, 50, 0);
// Start timer that updates status bar
SetTimer(UPDATE_STATUS_BAR_TIMER, 500, 0);
*/