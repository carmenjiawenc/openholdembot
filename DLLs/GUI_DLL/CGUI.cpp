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

#define GUI_DLL_EXPORTS

#include "CGUI.h"
#include <assert.h>
#include "dialog_about\DialogAbout.h"
#include "dialog_scraper_output\DialogScraperOutput.h"
#include "Toolbar\CFlagsToolbar.h"
#include "MainFrame\MainFrm.h"
#include "MainFrame\OpenHoldemDoc.h"
#include "MainFrame\OpenHoldemView.h"
#include "OpenHoldem_title\COpenHoldemTitle.h"
#include "statusbar\COpenHoldemStatusbar.h"
#include "Toolbar\CFlagsToolbar.h"
#include "..\Debug_DLL\debug.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineHandrank.h"
#include "..\WindowFunctions_DLL\window_functions.h"

BEGIN_MESSAGE_MAP(CGUI, CWnd)
  ON_COMMAND(ID_APP_ABOUT, &CGUI::OnAbout)
END_MESSAGE_MAP()

CGUI::CGUI() {
  _p_flags_toolbar = NULL;
  _p_main_frame = NULL;
  _p_openholdem_statusbar = NULL;
  _p_openholdem_title = NULL;
  _p_dialog_scraper_output = NULL;
  _p_white_infobox = NULL;
}

CGUI::~CGUI() {
}

void CGUI::Update() {
  OpenHoldemTitle()->UpdateTitle();
  if (DlgScraperOutput()) {
    DlgScraperOutput()->UpdateDisplay();
  }
  /// to do: Query custom log-message for white info-box
  // The main GUI gets created by another thread.
  // This can be slowed down if there are popups (parse-errors).
  // Handle the race-condition
  ///WAIT_FOR_CONDITION(MainFrame() != NULL);
  assert(MainFrame() != NULL);
	MainFrame()->ResetDisplay();
}

/// To do: detect and call
void CGUI::UpdateOnConnection() {
  ///FlagsToolbar()->ResetButtonsOnConnect();
}

void CGUI::UpdateOnDisconnection() {
  FlagsToolbar()->UnattachOHFromPokerWindow();
  FlagsToolbar()->ResetButtonsOnDisconnect();
}

CSingleDocTemplate* CGUI::CreateCSingleDocTemplate() {
  // Register the application's document templates.  Document templates
  // serve as the connection between documents, frame windows and views
  CSingleDocTemplate* pDocTemplate;
  // Document template and doc/view
  // https://msdn.microsoft.com/en-us/library/hts9a4xz.aspx
  // https://msdn.microsoft.com/en-us/library/d1e9fe7d.aspx
  write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to create CSingleDocTemplate()\n");
  pDocTemplate = new CSingleDocTemplate(
     IDR_MAINFRAME,
     RUNTIME_CLASS(COpenHoldemDoc),
     RUNTIME_CLASS(CMainFrame),	   // main SDI frame window
     RUNTIME_CLASS(COpenHoldemView));
  if (!pDocTemplate) {
    write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Creating CSingleDocTemplate() failed\n");
    return NULL;
  }
  return pDocTemplate;
}

void CGUI::CreateFlagsToolbar(CFrameWnd *parent_window) {
  _p_flags_toolbar = new CFlagsToolbar(parent_window);
}

void CGUI::CreateStatusbar(CFrameWnd *parent_window) {
  _p_openholdem_statusbar = new COpenHoldemStatusbar(parent_window);
}

void CGUI::CreateDialogScraperOutput(CFrameWnd *parent_window) {
  MessageBox_Interactive("Please note:\n"
    "OpenScrape scrapes everything, but OpenHoldem is optimized\n"
    "to scrape only necessary info.\n"
    "\n"
    "For example:\n"
    "If a players first card is \"cardback\" we don't even have to scrape the second one.\n"
    "This is a feature, not a bug.\n",
    "Info", 0);
  write_log(Preferences()->debug_gui(), "[GUI] Going to create scraper output dialog\n");
  _p_dialog_scraper_output = new CDlgScraperOutput(this);
  write_log(Preferences()->debug_gui(), "[GUI] Scraper output dialog: step 1 finished\n");
  DlgScraperOutput()->Create(CDlgScraperOutput::IDD, this);
  write_log(Preferences()->debug_gui(), "[GUI] Scraper output dialog: step 2 finished\n");
  DlgScraperOutput()->ShowWindow(SW_SHOW);
  write_log(Preferences()->debug_gui(), "[GUI] Scraper output dialog: step 3 finished\n");
  /// here?
  FlagsToolbar()->EnableButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, true);
  write_log(Preferences()->debug_gui(), "[GUI] Scraper output dialog: step 4 (final) finished\n");
}

void CGUI::OnAbout() {
  CDlgAbout aboutDlg;
  aboutDlg.DoModal();
}

CGUI* gui = NULL;

CGUI* GUI() {
  if (gui == NULL) {
    // Lazy initialization 
    gui = new CGUI;
  }
  return gui;
}


// Reset display
/// not here -> heartbeat
///InvalidateRect(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), NULL, true);