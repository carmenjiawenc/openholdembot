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

#include <afxwin.h>
#include "LibDef.h"
#include "formula_editor\DialogFormulaScintilla.h"
#include "dialog_scraper_output\DialogScraperOutput.h"
#include "Toolbar\CFlagsToolbar.h"
#include "MainFrame\MainFrm.h"
#include "statusbar\COpenHoldemStatusbar.h"
#include "OpenHoldem_title\COpenHoldemTitle.h"
#include "white_info_box\CWhiteInfoBox.h"

class CSingleDocTemplate;

class GUI_DLL_API CGUI: public CWnd {
public:
  CGUI();
  ~CGUI();
public:
  void Update();
public:
  CFlagsToolbar *FlagsToolbar() {
    return _p_flags_toolbar;
  }
  CMainFrame *MainFrame() {
    return _p_main_frame;
  }
  COpenHoldemStatusbar* OpenHoldemStatusbar() {
    return _p_openholdem_statusbar;
  }
  COpenHoldemTitle* OpenHoldemTitle() {
    return _p_openholdem_title;
  }
  CDlgFormulaScintilla* DlgFormulaScintilla() {
    return _p_dialog_formula_scintilla;
  }
  CDlgScraperOutput *DlgScraperOutput() {
    return _p_dialog_scraper_output;
  }
  CWhiteInfoBox* WhiteInfoBox() { 
    return _p_white_infobox;
  }
public:
  CSingleDocTemplate* CreateCSingleDocTemplate();
protected:
  DECLARE_MESSAGE_MAP()
private:
  void UpdateOnConnection();
  void UpdateOnDisconnection();
private:
  afx_msg void OnAbout();
private:
  ///!!! init
  CFlagsToolbar* _p_flags_toolbar;
  CMainFrame* _p_main_frame;
  COpenHoldemStatusbar* _p_openholdem_statusbar; 
  COpenHoldemTitle* _p_openholdem_title;
  CDlgFormulaScintilla* _p_dialog_formula_scintilla;
  CDlgScraperOutput* _p_dialog_scraper_output;
  CWhiteInfoBox* _p_white_infobox;
};

GUI_DLL_API CGUI* GUI();




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