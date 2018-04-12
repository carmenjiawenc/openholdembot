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

#include "COpenHoldemStatusbar.h"
#include "..\resource.h"
#include "..\..\Symbols_DLL\CEngineContainer.h"
#include "..\..\Symbols_DLL\CFunctionCollection.h"
#include "..\..\Scraper_DLL\CBasicScraper.h"
#include "..\..\Symbols_DLL\CSymbolEngineAutoplayer.h"
#include "..\..\Symbols_DLL\CSymbolEngineHandrank.h"
#include "..\..\Symbols_DLL\CSymbolEngineIsOmaha.h"
#include "..\..\Symbols_DLL\CSymbolEnginePrwin.h"
#include "..\..\Symbols_DLL\CSymbolEngineTime.h"
#include "..\..\Symbols_DLL\CSymbolEngineUserchair.h"
#include "..\..\StringFunctions_DLL\string_functions.h"
#include "..\..\Tablestate_DLL\TableState.h"

COpenHoldemStatusbar::COpenHoldemStatusbar(CWnd *main_window) {
  _main_window = main_window;
  InitStatusbar();
  SetLastAction("");
  SetPrWin(0, 0, 0);
  SetHandrank(0);
}

COpenHoldemStatusbar::~COpenHoldemStatusbar() {
}

void COpenHoldemStatusbar::InitStatusbar() {
  _status_bar.Create(_main_window);
  static UINT indicators[] = {
    ID_INDICATOR_STATUS_ACTION,
    ID_INDICATOR_STATUS_DUMMY,
    ID_INDICATOR_STATUS_HANDRANK,
    ID_INDICATOR_STATUS_PRWIN,
  };
  _status_bar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
  int position = 0;
  _status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_ACTION, NULL, 100);
  ++position;
  _status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_HANDRANK, NULL, 100);
  ++position;
  _status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_PRWIN, NULL, 100);
  ++position;
  _status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_DUMMY, SBPS_STRETCH, 280);
  ++position;
}

void COpenHoldemStatusbar::GetWindowRect(RECT *statusbar_position) {
  _status_bar.GetWindowRect(statusbar_position);
}

void COpenHoldemStatusbar::OnUpdateStatusbar() {
  if (TableState()->User()->HasKnownCards()){
    // Format data for display
    // Handrank
    _status_handrank.Format("%i/169", _handrank);
    // PrWin: percentages instead of probabilities
    _status_prwin.Format("PrWin %3.1f/%3.1f/%3.1f",
      100 * _prwin, 100 * _prtie, 100 * _prlos);
  } else if (EngineContainer()->symbol_engine_time()->elapsedauto() > 5) {
    // Reset display 5 seconds after last action
    SetPrWin(0, 0, 0);
    SetHandrank(0);
    _status_handrank = "";
    _status_prwin = "";
    _last_action = "";
  } else {
    // No cards, but shortly after last action
    // Don't change anything, keep information
  }
  // Display data
  _status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_ACTION), LastAction());
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_HANDRANK), _status_handrank);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PRWIN), _status_prwin);
}

CString COpenHoldemStatusbar::LastAction() {
  if (EngineContainer()->symbol_engine_userchair() == NULL)	{
		// Very early phase of initialization
		// Can't continue here.
		return "Not playing";
	}
	if (!EngineContainer()->symbol_engine_userchair()->userchair_confirmed()) 	{
		return "Not playing";
	}
  // Return the last saved action.
  // This value should get set exactly once after autoplayer/actions
  // to avoid multiple evaluations of the autoplayer-functions,
  // especially at different heartbeats.
  return _last_action;
}

void COpenHoldemStatusbar::SetPrWin(double prwin, double prtie, double prlos) {
  _prwin = prwin;
  _prtie = prtie;
  _prlos = prlos;
}