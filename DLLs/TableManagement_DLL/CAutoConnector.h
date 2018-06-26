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
// Purpose: automatically connecting to unserved poker-tables,
//   using shared memory and a mutex to synchronize with other instaces.
//
//******************************************************************************

#include "LibDef.h"
#include <afxmt.h>
#include <afxwin.h>
#include "CPopupHandler.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
///#include "CSpaceOptimizedGlobalObject.h"

class TABLE_MANAGEMENT_DLL_API CAutoConnector /*#: public CSpaceOptimizedGlobalObject */{
 public:
	CAutoConnector();
	~CAutoConnector();
 public:
	bool Connect(HWND targetHWnd);
	void Disconnect(CString reason_for_disconnection);
	double SecondsSinceLastFailedAttemptToConnect(); 
 public:
  bool IsConnectedToAnything();
  bool IsConnectedToExistingWindow();
  bool IsConnectedToGoneWindow();
 public:
	// public accessors
	const HWND attached_hwnd()    { return _attached_hwnd; }
 private:
	int SelectTableMapAndWindowAutomatically();
	void WriteLogTableReset(CString event_and_reason);
 private:
	void Check_TM_Against_All_Windows_Or_TargetHWND(int tablemap_index, HWND targetHWnd);
  void CheckIfWindowMatchesMoreThanOneTablemap(HWND hwnd);
 private:
  void set_attached_hwnd(const HWND table);
 private:
	void FailedToConnectBecauseNoWindowInList();
	void FailedToConnectProbablyBecauseAllTablesAlreadyServed();
	void GoIntoPopupBlockingMode();
 private:
  CPopupHandler popup_handler;
 private:
	// private variables - use public accessors and public mutators to address these
	HWND     _attached_hwnd; // Table that we are attached to
	CCritSec m_critsec;
 private:
	// Mutex used for cross-instance autoconnector coordination
	CMutex *_autoconnector_mutex;
};