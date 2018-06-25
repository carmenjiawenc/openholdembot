//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Watching for crashed processes and cleaning up shared memory
//   * so that another instance can connect to this table
//   * because the OpenHoldem starter needs the number of running processes
//
//******************************************************************************

#define PROCESS_MANAGEMEMT_DLL_EXPORTS

#include "CWatchdog.h"
#include "..\Debug_DLL\debug.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\SessionCounter_DLL\CSessionCounter.h"
#include "..\TableManagement_DLL\CSharedMem.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

// CWatchdog uses the same shared memory segment like CSharedMem
#pragma data_seg(kOpenHoldemSharedmemorySegment) // names are limited to 8 chars, including the dot.
__declspec(allocate(kOpenHoldemSharedmemorySegment)) static	time_t timestamps_openholdem_alive[MAX_SESSION_IDS] = { NULL };
#pragma data_seg()
#pragma comment(linker, "/SECTION:.ohshmem,RWS")		// RWS: read, write, shared

const int kSecondsToconsiderAProcessAsFrozen = 15;

CWatchdog::CWatchdog() {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] CWatchdog()\n");
  MarkThisInstanceAsAlive();
}

CWatchdog::~CWatchdog() {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] ~CWatchdog()\n");
  MarkThisInstanceAsDead();
}

void CWatchdog::HandleCrashedAndFrozenProcesses() {
  assert(TableManagement()->SharedMem() != NULL);
  if (TableManagement()->SharedMem()->OpenHoldemProcessID() == 0) {
    write_log(k_always_log_errors, "WARNING! Watch-dog turned off, unavailable process ID\n");
    return;
  }
  MarkThisInstanceAsAlive();
  WatchForCrashedProcesses();
  WatchForFrozenProcesses();
}

void CWatchdog::MarkInstanceAsAlive(int session_ID) {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] Marking instance %d alive\n", session_ID);
  assert(session_ID >=  0);
  assert(session_ID < MAX_SESSION_IDS);
  time_t current_time;
  time(&current_time);
  timestamps_openholdem_alive[session_ID] = current_time;
}

void CWatchdog::MarkThisInstanceAsAlive() {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] Marking this instance alive\n");
  assert(SessionCounter() != NULL);
  MarkInstanceAsAlive(SessionCounter()->session_id());
}

void CWatchdog::MarkInstanceAsDead(int session_ID) {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] Marking instance %d dead\n", session_ID);
  assert(session_ID >= 0);
  assert(session_ID < MAX_SESSION_IDS);
  timestamps_openholdem_alive[session_ID] = kUndefinedZero;
}

void CWatchdog::MarkThisInstanceAsDead() {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] Marking this instance dead\n");
  assert(SessionCounter() != NULL);
  MarkInstanceAsDead(SessionCounter()->session_id());
}

void CWatchdog::WatchForCrashedProcesses() {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] Watching for crashed processes\n");
  for (int i = 0; i < MAX_SESSION_IDS; ++i) {
    if (TableManagement()->SharedMem()->IsDeadOpenHoldemProcess(i)) {
      write_log(Preferences()->debug_watchdog(), "[CWatchdog] Found crashed process and cleaning up\n");
      MarkInstanceAsDead(i);;
      TableManagement()->SharedMem()->CleanUpProcessMemory(i);
    }
  }
}

// !! move to dll
BOOL KillProcess(DWORD dwProcessId) {
  // http://stackoverflow.com/questions/2443738/c-terminateprocess-function
  DWORD dwDesiredAccess = PROCESS_TERMINATE;
  BOOL  bInheritHandle = FALSE;
  INT   uExitCode = -1;
  HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
  if (hProcess == NULL) {
    return FALSE;
  }
  BOOL result = TerminateProcess(hProcess, uExitCode);
  CloseHandle(hProcess);
  return result;
}

void CWatchdog::WatchForFrozenProcesses() {
  write_log(Preferences()->debug_watchdog(), "[CWatchdog] Watching for frozen processes\n");
  time_t current_time;
  time(&current_time);
  for (int i = 0; i < MAX_SESSION_IDS; ++i) {
    if (TableManagement()->SharedMem()->OpenHoldemProcessID(i) == 0) {
      // Not a process
      continue;
    }
    time_t last_process_timestamp = timestamps_openholdem_alive[i];
    int seconds_elapsed = current_time - last_process_timestamp;
    assert(seconds_elapsed >= 0);
    if (seconds_elapsed > kSecondsToconsiderAProcessAsFrozen) {
      if (seconds_elapsed > 2 * kSecondsToconsiderAProcessAsFrozen) {
        // Differences between time-stamp and currernt time way too large,
        // we should already have killed it.
        // Probably a new process which does not yet proper heartbeating,
        // fix its time-stamp and grant it some time to continue.
        write_log(Preferences()->debug_watchdog(), "[CWatchdog] Deep freeze detected %i, PID: %i\n",
          i, TableManagement()->SharedMem()->OpenHoldemProcessID(i));
        write_log(Preferences()->debug_watchdog(), "[CWatchdog] Might be stale data\n");
        write_log(Preferences()->debug_watchdog(), "[CWatchdog] Granting more time\n");
        TableManagement()->SharedMem()->OpenHoldemProcessID(i);
        MarkInstanceAsAlive(i);
        continue;
      }
#ifndef _DEBUG
      write_log(Preferences()->debug_watchdog(), "[CWatchdog] Killing frozen process %i, PID: %i\n",
        i, TableManagement()->SharedMem()->OpenHoldemProcessID(i));
      KillProcess(TableManagement()->SharedMem()->OpenHoldemProcessID(i));
      MarkInstanceAsDead(i);
      TableManagement()->SharedMem()->CleanUpProcessMemory(i);
#else
      // Don't kill any processes in debug.mode
      // It is extremely annoying if we hit a breakpoint
      // and another instance kills the paused program.
#endif _DEBUG
      write_log(Preferences()->debug_watchdog(), 
        "[CWatchdog] Skipped killing frozen process %i, PID: %i because of debug-mode\n",
        i, TableManagement()->SharedMem()->OpenHoldemProcessID(i));
    }
  }
}