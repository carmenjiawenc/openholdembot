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
#include "dialog_scraper_output\DialogScraperOutput.h"
#include "MainFrame\MainFrm.h"
#include "OpenHoldem_title\COpenHoldemTitle.h"
#include "statusbar\COpenHoldemStatusbar.h"
#include "Toolbar\CFlagsToolbar.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineHandrank.h"

CGUI::CGUI() {
}

CGUI::~CGUI() {
}

void CGUI::Update() {
  OpenHoldemTitle()->UpdateTitle();
  assert(MainFrame() != NULL);
  MainFrame()->ResetDisplay();
  if (DlgScraperOutput()) {
    DlgScraperOutput()->UpdateDisplay();
  }
  ///OpenHoldemStatusbar()->SetHandrank(_handrank169);
  /// To do: set and clear last action in statusbar
  /// Query custom log-message for white info-box
  ///
  /// Update buttons, menus and toolbars
  /// Uncheck formula button on main toolbar
  /// GUI()->FlagsToolbar()->CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);

  // Reset "ScraperOutput" dialog, if it is live
  //if (GUI()->DlgScraperOutput()) {
  //  GUI()->DlgScraperOutput()->Reset();
  //}
  //
  // Reset "ScraperOutput" dialog, if it is live
  /*#if (GUI()->DlgScraperOutput()) {
  GUI()->DlgScraperOutput()->Reset();
  }*/
  /*// The main GUI gets created by another thread.
      // This can be slowed down if there are popups (parse-errors).
      // Handle the race-condition
      // !!!!! create timer function unmain-frame instead
      ///WAIT_FOR_CONDITION(PMainframe() != NULL)
      ///assert(PMainframe() != NULL);
			// Reset display
			///PMainframe()->ResetDisplay();*/
}

/// To do: detect and call
void CGUI::UpdateOnConnection() {
  FlagsToolbar()->ResetButtonsOnConnect();
}

void CGUI::UpdateOnDisconnection() {
  FlagsToolbar()->UnattachOHFromPokerWindow();
  FlagsToolbar()->ResetButtonsOnDisconnect();
}

#include "MainFrame\OpenHoldemDoc.h"
#include "MainFrame\OpenHoldemView.h"

CSingleDocTemplate* CGUI::CreateCSingleDocTemplate() {
  // Register the application's document templates.  Document templates
  // serve as the connection between documents, frame windows and views
  CSingleDocTemplate* pDocTemplate;
  // Document template and doc/view
  // https://msdn.microsoft.com/en-us/library/hts9a4xz.aspx
  // https://msdn.microsoft.com/en-us/library/d1e9fe7d.aspx
  ///write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to create CSingleDocTemplate()\n");
  pDocTemplate = new CSingleDocTemplate(
     IDR_MAINFRAME,
     RUNTIME_CLASS(COpenHoldemDoc),
     RUNTIME_CLASS(CMainFrame),	   // main SDI frame window
     RUNTIME_CLASS(COpenHoldemView));
  if (!pDocTemplate) {
    ///write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Creating CSingleDocTemplate() failed\n");
    return NULL;
  }
  ///write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to AddDocTemplate()\n");
  ///AddDocTemplate(pDocTemplate);*/
  return pDocTemplate;
}

CGUI* gui = NULL;

CGUI* GUI() {
  if (gui == NULL) {
    // Lazy initialization 
    gui = new CGUI;
  }
  return gui;
}