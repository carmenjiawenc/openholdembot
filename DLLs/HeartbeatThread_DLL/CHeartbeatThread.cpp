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

#define HEARTBEATTHREAD_DLL_EXPORTS

#include "CHeartbeatThread.h"
#include <process.h>
#include "CAutoplayer.h"
#include "CHeartbeatDelay.h"
#include "CValidator.h"
#include "..\Debug_DLL\debug.h"
#include "..\GUI_DLL\CGUI.h"
#include "..\MemoryManagement_DLL\MemoryLogging.h"
#include "..\OpenHoldem_GamestatePopulator_DLL\COpenHoldemGamestatePopulator.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\ProcessManagement\COpenHoldemStarter.h"
#include "..\ProcessManagement\CProcessManagement.h"
#include "..\ProcessManagement\CWatchdog.h"
#include "..\StringFunctions_DLL\string_functions.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineAutoplayer.h"
#include "..\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\Symbols_DLL\CSymbolEngineUserchair.h"
#include "..\TableManagement_DLL\CAutoConnector.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\Tablestate_DLL\CTableTitle.h"

///CHeartbeatThread	 HeartbeatThread = NULL;
CRITICAL_SECTION	 CHeartbeatThread::cs_update_in_progress;
long int			     CHeartbeatThread::_heartbeat_counter = 0;
CHeartbeatThread   *CHeartbeatThread::pParent = NULL;
CHeartbeatDelay    CHeartbeatThread::_heartbeat_delay;
COpenHoldemGamestatePopulator* CHeartbeatThread::_gamestate_populator = NULL;

CAutoplayer* _autoplayer; //!!!

CHeartbeatThread::CHeartbeatThread() {
	InitializeCriticalSectionAndSpinCount(&cs_update_in_progress, 4000);
  _heartbeat_counter = 0;
  _gamestate_populator = new COpenHoldemGamestatePopulator; ///!!! clean up
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
	///OpenHoldem()->HeartbeatThread() = NULL;
}

void CHeartbeatThread::StartThread() {
	// Start thread
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Starting heartbeat thread\n");
    assert(this != NULL);
	AfxBeginThread(HeartbeatThreadFunction, this);
}

UINT CHeartbeatThread::HeartbeatThreadFunction(LPVOID pParam) {
	pParent = static_cast<CHeartbeatThread*>(pParam);
  assert(pParent != NULL);
  CProcessManagement process_management;
  ///CTablepointChecker tablepoint_checker;
	// Seed the RNG
	srand((unsigned)GetTickCount());
	while (true) {
		_heartbeat_counter++;
		write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Starting next cycle\n");
		// Check event for stop thread
		if(::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0) {
			// Set event
      write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Ending heartbeat thread\n");
			::SetEvent(pParent->_m_wait_thread);
			AfxEndThread(0);
		}
    ///assert(OpenHoldem()->TablemapLoader() != NULL);
		///OpenHoldem()->TablemapLoader()->ReloadAllTablemapsIfChanged();
    assert(TableManagement()->AutoConnector() != NULL);
    write_log(Preferences()->debug_alltherest(), "[CHeartbeatThread] location Johnny_B\n");
    if (TableManagement()->AutoConnector()->IsConnectedToGoneWindow()) {
      TableManagement()->AutoConnector()->Disconnect("table disappeared");
    }
    if (!TableManagement()->AutoConnector()->IsConnectedToAnything()) {
      // Not connected
      AutoConnect();
    }
    // No "else" here
    // We want one fast scrape immediately after connection
    // without any heartbeat-sleeping.
    write_log(Preferences()->debug_alltherest(), "[CHeartbeatThread] location Johnny_C\n");
		if (TableManagement()->AutoConnector()->IsConnectedToExistingWindow()) {
     /*# if (tablepoint_checker.TablepointsMismatchedTheLastNHeartbeats()) {
        TableManagement()->AutoConnector()->Disconnect("table theme changed (tablepoints)");
      } else*/ {
        TableManagement()->TablePositioner()->AlwaysKeepPositionIfEnabled();
        ScrapeEvaluateAct();
      } 		
		}
    process_management.Watchdog()->HandleCrashedAndFrozenProcesses();
    if (Preferences()->use_auto_starter()) {
      process_management.OpenHoldemStarter()->StartNewInstanceIfNeeded();
    }
    if (Preferences()->use_auto_shutdown()) {
      process_management.OpenHoldemStarter()->CloseThisInstanceIfNoLongerNeeded();
    }
    _heartbeat_delay.FlexibleSleep();
		write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Heartbeat cycle ended\n");
    LogMemoryUsage("Heartbeat cycle");
	}
}

void CHeartbeatThread::ScrapeEvaluateAct() {
	// This critical section lets other threads know that the internal state is being updated
	EnterCriticalSection(&pParent->cs_update_in_progress);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Scrape window
  
  write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Calling DoScrape.\n");
  _gamestate_populator->Populate();
  // We must not check if the scrape of the table changed, because:
  //   * some symbol-engines must be evaluated no matter what
  //   * we might need to act (sitout, ...) on empty/non-changing tables
  //   * auto-player needs stable frames too
	EngineContainer()->EvaluateAll();
	// Replay-frames no longer here in the heartbeat.
  // we have a "ReplayFrameController for that.
  LeaveCriticalSection(&pParent->cs_update_in_progress);
  /// check if gui exists
	GUI()->Update();
	////////////////////////////////////////////////////////////////////////////////////////////
	// OH-Validator
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Calling Validator.\n");
  CValidator validator; //???
  validator.Validate();
	////////////////////////////////////////////////////////////////////////////////////////////
	// Autoplayer
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] autoplayer_engaged(): %s\n", 
		Bool2CString(_autoplayer->autoplayer_engaged()));
	write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] EngineContainer()->symbol_engine_userchair()->userchair()_confirmed(): %s\n", 
		Bool2CString(EngineContainer()->symbol_engine_userchair()->userchair_confirmed()));
	// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
	if (_autoplayer->autoplayer_engaged()) {
		write_log(Preferences()->debug_heartbeat(), "[HeartBeatThread] Calling DoAutoplayer.\n");
		_autoplayer->DoAutoplayer();
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