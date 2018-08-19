//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: starting and terminating bots automatically
//
//******************************************************************************

#define PROCESS_MANAGEMEMT_DLL_EXPORTS

#include "COpenHoldemStarter.h"
#include "..\Debug_DLL\debug.h"
#include "..\Files_DLL\Files.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\SessionCounter_DLL\CSessionCounter.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineTime.h"
#include "..\TableManagement_DLL\CSharedMem.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

// For connection and popup handling
const int kMinNumberOfUnoccupiedBotsNeeded =   1;
const int kSecondsToWaitBeforeTermination  = 120;
const int kSecondsToWaitBeforeNextStart    =   5;

COpenHoldemStarter::COpenHoldemStarter() {
  time(&_starting_time_of_last_instance);
}

COpenHoldemStarter::~COpenHoldemStarter() {
}

void COpenHoldemStarter::StartNewInstanceIfNeeded() {
  assert(TableManagement()->SharedMem() != NULL);
  if (TableManagement()->SharedMem()->OpenHoldemProcessID() == 0) {
    write_log(k_always_log_errors, "WARNING! Auto-starter turned off, unavailable process ID\n");
    return;
  }
  if (TableManagement()->SharedMem()->NUnoccupiedBots() >= kMinNumberOfUnoccupiedBotsNeeded) {
    // Enough instance available for new connections / popup handling
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] No bots needed, enough free instances.\n");
    return;
  }
  time_t current_time;
  time(&current_time);
  if (current_time - _starting_time_of_last_instance < kSecondsToWaitBeforeNextStart) {
    // Another instance got started only recently.
    // Grant it some seconds to show up
    // and don't flood the screen with new bots.
    return;
  }
  if (TableManagement()->SharedMem()->LowestConnectedSessionID() != SessionCounter()->session_id()) {
    // Only one instance should handle auto-starting.
    // This might delay auto-starting until the first connection, which is OK.
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Not my business to start new instances.\n");
    return;
  }
  time(&_starting_time_of_last_instance);
  //! delay until next start
  // No error-checking, as Openholdem exists (at least when we started).
  // http://msdn.microsoft.com/en-us/library/windows/desktop/bb762153%28v=vs.85%29.aspx
  write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Starting new instance [%s].\n",
    ExecutableFilename());
  ShellExecute(
    NULL,               // Pointer to parent window; not needed
    "open",             // "open" == "execute" for an executable
    ExecutableFilename(),
    NULL, 		          // Parameters
    "",                 // Working directory
    SW_SHOWNORMAL);		  // Active window, default size
}

void COpenHoldemStarter::CloseThisInstanceIfNoLongerNeeded() {
  assert(TableManagement()->SharedMem() != NULL);
  if (TableManagement()->SharedMem()->OpenHoldemProcessID() == 0) {
    write_log(k_always_log_errors, "WARNING! Auto-shutdown turned off, unavailable process ID\n");
    return;
  }
  if (TableManagement()->AutoConnector()->IsConnectedToAnything()) {
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Playing, therefore still needed.\n");
    // Instance needed for playing
    return;
  }
  if (TableManagement()->SharedMem()->NUnoccupiedBots() <= kMinNumberOfUnoccupiedBotsNeeded) {
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Needed for new connections.\n");
    // Instance needed for new connections / popup handling
    return;
  }
  if (EngineContainer()->symbol_engine_time()->elapsedauto() < kSecondsToWaitBeforeTermination) {
    // Don't shut down immediately
    // Instance might be needed soon again
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Not waited long enough for shutdown.\n");
    return;
  }
  if (TableManagement()->SharedMem()->LowestUnconnectedSessionID() != SessionCounter()->session_id()) {
    // Only one instance should tzerminate at a time
    // to keep one instance available
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Not my turn to shutdown.\n");
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Lowest free ID: %d\n", TableManagement()->SharedMem()->LowestUnconnectedSessionID());
    write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] My ID: %d\n", SessionCounter()->session_id());
    return;
  }
  write_log(Preferences()->debug_autostarter(), "[COpenHoldemStarter] Shutting down this instance.\n");
   ///!!! Might no longer work without window
  PostMessage(AfxGetApp()->m_pActiveWnd->GetSafeHwnd(),
    WM_QUIT, NULL, NULL);
}