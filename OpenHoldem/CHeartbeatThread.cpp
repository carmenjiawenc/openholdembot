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

#include "srdafx.h"
#include "CHeartbeatThread.h"
#include <process.h>
#include "CAutoconnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CHeartbeatDelay.h"
#include "CIteratorThread.h"
#include "COpenHoldemHopperCommunication.h"
#include "COpenHoldemStarter.h"
#include "COpenHoldemStatusbar.h"
#include "COpenHoldemTitle.h"
#include "CTableMapLoader.h"
#include "CTablepointChecker.h"
#include "CTablePositioner.h"
#include "CValidator.h"
#include "DialogScraperOutput.h"
#include "MainFrm.h"
#include "MemoryLogging.h"
#include "OpenHoldem.h"
#include "..\DLLs\Scraper_DLL\CLazyScraper.h"
#include "..\DLLs\Scraper_DLL\CScraper.h"
#include "..\DLLs\Symbols_DLL\CBetroundCalculator.h"
#include "..\DLLs\Symbols_DLL\CEngineContainer.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineAutoplayer.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineUserchair.h"
#include "..\CTablemap\CTablemap.h"
#include "..\DLLs\Tablestate_DLL\CTableTitle.h"

CHeartbeatThread	 *OpenHoldem()->HeartbeatThread() = NULL;
CRITICAL_SECTION	 CHeartbeatThread::cs_update_in_progress;
long int			     CHeartbeatThread::_heartbeat_counter = 0;
CHeartbeatThread   *CHeartbeatThread::pParent = NULL;
CHeartbeatDelay    CHeartbeatThread::_heartbeat_delay;
COpenHoldemStarter CHeartbeatThread::_openholdem_starter;

CHeartbeatThread::CHeartbeatThread() {
	InitializeCriticalSectionAndSpinCount(&cs_update_in_progress, 4000);
  _heartbeat_counter = 0;
  // Create events
	_m_stop_thread = CreateEvent(0, TRUE, FALSE, 0);
	_m_wait_thread = CreateEvent(0, TRUE, FALSE, 0);
}

CHeartbeatThread::~CHeartbeatThread() {
	// Trigger thread to stop
	::SetEvent(_m_stop_thread);

	// Wait until thread finished
	::WaitForSingleObject(_m_wait_thread, k_max_time_to_wait_for_thread_to_shutdown);

	// Close handles
	::CloseHandle(_m_stop_thread);
	::CloseHandle(_m_wait_thread);

	DeleteCriticalSection(&cs_update_in_progress);
	OpenHoldem()->HeartbeatThread() = NULL;
}

void CHeartbeatThread::StartThread() {
	// Start thread
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Starting heartbeat thread\n");
    assert(this != NULL);
	AfxBeginThread(HeartbeatThreadFunction, this);
}

UINT CHeartbeatThread::HeartbeatThreadFunction(LPVOID pParam) {
  CTablepointChecker tablepoint_checker;
	pParent = static_cast<CHeartbeatThread*>(pParam);
  assert(pParent != NULL);
	// Seed the RNG
	srand((unsigned)GetTickCount());

	while (true) {
		_heartbeat_counter++;
		write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Starting next cycle\n");
		// Check event for stop thread
		if(::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0) {
			// Set event
      write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Ending heartbeat thread\n");
      LogMemoryUsage("Hc");
			::SetEvent(pParent->_m_wait_thread);
			AfxEndThread(0);
		}
    assert(OpenHoldem()->TablemapLoader() != NULL);
    LogMemoryUsage("H1");
		OpenHoldem()->TablemapLoader()->ReloadAllTablemapsIfChanged();
    LogMemoryUsage("H2");
    assert(TableManagement()->AutoConnector() != NULL);
    write_log(Preferences()->debug_alltherest(), "[CHeartbeatThread] location Johnny_B\n");
    if (TableManagement()->AutoConnector()->IsConnectedToGoneWindow()) {
      LogMemoryUsage("H3");
      TableManagement()->AutoConnector()->Disconnect("table disappeared");
    }
    LogMemoryUsage("H4");
    if (!TableManagement()->AutoConnector()->IsConnectedToAnything()) {
      // Not connected
      AutoConnect();
    }
    // No "else" here
    // We want one fast scrape immediately after connection
    // without any heartbeat-sleeping.
    LogMemoryUsage("H5");
    write_log(Preferences()->debug_alltherest(), "[CHeartbeatThread] location Johnny_C\n");
		if (TableManagement()->AutoConnector()->IsConnectedToExistingWindow()) {
      if (tablepoint_checker.TablepointsMismatchedTheLastNHeartbeats()) {
        LogMemoryUsage("H6");
        TableManagement()->AutoConnector()->Disconnect("table theme changed (tablepoints)");
      } else {
        LogMemoryUsage("H7");
        ScrapeEvaluateAct();
      } 		
		}
    LogMemoryUsage("H8");
    OpenHoldem()->WatchDog()->HandleCrashedAndFrozenProcesses();
    if (Preferences()->use_auto_starter()) {
      LogMemoryUsage("H9");
      _openholdem_starter.StartNewInstanceIfNeeded();
    }
    LogMemoryUsage("Ha");
    if (Preferences()->use_auto_shutdown()) {
      _openholdem_starter.CloseThisInstanceIfNoLongerNeeded();
    }
    LogMemoryUsage("Hb");
    _heartbeat_delay.FlexibleSleep();
		write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Heartbeat cycle ended\n");
    LogMemoryUsage("End of heartbeat cycle");
	}
}

void CHeartbeatThread::ScrapeEvaluateAct() {
	TablePositioner()->AlwaysKeepPositionIfEnabled();
	// This critical section lets other threads know that the internal state is being updated
	EnterCriticalSection(&pParent->cs_update_in_progress);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Scrape window
  TableState()->TableTitle()->UpdateTitle();
  write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Calling DoScrape.\n");
  p_lazyscraper->DoScrape();
  // We must not check if the scrape of the table changed, because:
  //   * some symbol-engines must be evaluated no matter what
  //   * we might need to act (sitout, ...) on empty/non-changing tables
  //   * auto-player needs stable frames too
	EngineContainer()->EvaluateAll();
	// Reply-frames no longer here in the heartbeat.
  // we have a "ReplayFrameController for that.
  LeaveCriticalSection(&pParent->cs_update_in_progress);
	GUI()->OpenHoldemTitle()->UpdateTitle();
	////////////////////////////////////////////////////////////////////////////////////////////
	// Update scraper output dialog if it is present
	if (GUI()->DlgScraperOutput()) {
		GUI()->DlgScraperOutput()->UpdateDisplay();
	}
  
	////////////////////////////////////////////////////////////////////////////////////////////
	// OH-Validator
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Calling Validator.\n");
  p_validator->Validate();

	////////////////////////////////////////////////////////////////////////////////////////////
	// Autoplayer
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] autoplayer_engaged(): %s\n", 
		Bool2CString(p_autoplayer->autoplayer_engaged()));
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] EngineContainer()->symbol_engine_userchair()->userchair()_confirmed(): %s\n", 
		Bool2CString(EngineContainer()->symbol_engine_userchair()->userchair_confirmed()));
	// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
	if (p_autoplayer->autoplayer_engaged()) {
		write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Calling DoAutoplayer.\n");
		p_autoplayer->DoAutoplayer();
	}
}

void CHeartbeatThread::AutoConnect() {
  write_log(Preferences()->debug_alltherest(), "[CHeartbeatThread] location Johnny_D\n");
	assert(!TableManagement()->AutoConnector()->IsConnectedToAnything());
	if (Preferences()->autoconnector_when_to_connect() == k_AutoConnector_Connect_Permanent) {
		if (TableManagement()->AutoConnector()->SecondsSinceLastFailedAttemptToConnect() > 1 /* seconds */) {
			write_log(Preferences()->debug_autoconnector(), "[CHeartbeatThread] going to call Connect()\n");
			TableManagement()->AutoConnector()->Connect(NULL);
		}	else {
			write_log(Preferences()->debug_autoconnector(), "[CHeartbeatThread] Reconnection blocked. Other instance failed previously.\n");
		}
	}
}