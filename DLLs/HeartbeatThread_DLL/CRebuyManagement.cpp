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

#include "CRebuyManagement.h"
#include <atlstr.h>
#include <time.h>
///#include "COcclusionCheck.h"
#include "..\Debug_DLL\debug.h"
#include "..\Formula_DLL\CFormula.h"
#include "..\Formula_DLL\CFunctionCollection.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CHandresetDetector.h"
#include "..\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\Symbols_DLL\CSymbolEngineUserchair.h"
#include "..\Symbols_DLL\CSymbolEngineTableLimits.h"
#include "..\TableManagement_DLL\CAutoConnector.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\Tablestate_DLL\TableState.h"
#include "..\WindowFunctions_DLL\window_functions.h"

CRebuyManagement::CRebuyManagement() {
	// Init time of last rebuy in a reasonable way at startup.
	time(&RebuyLastTime);
	PreviousRebuyHandNumber = "";
}

CRebuyManagement::~CRebuyManagement() {
}


bool CRebuyManagement::MinimumDelayElapsed() {
	unsigned int MinimumTimeDifference = Preferences()->rebuy_minimum_time_to_next_try();
	// Make sure, we don't try to rebuy too often in a short time
	time(&CurrentTime);
	double RebuyTimeDifference = difftime(CurrentTime, RebuyLastTime);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Time since last rebuy %.2f\n", RebuyTimeDifference);
	if (RebuyTimeDifference < MinimumTimeDifference) {
		write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] MinimumDelayElapsed(): false\n");
		return false;
	}
	return true;
}

bool CRebuyManagement::ChangeInHandNumber() {
  if (!Preferences()->rebuy_condition_change_in_handnumber()) {
    return true;
  }
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] handnumber %s\n", 
    PreviousRebuyHandNumber);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] handnumber %s\n", 
    EngineContainer()->HandresetDetector()->GetHandNumber());
  if (EngineContainer()->HandresetDetector()->GetHandNumber() > PreviousRebuyHandNumber) {
    write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] ChangeInHandNumber(): true\n");
		return true;
	}
	write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] ChangeInHandNumber(): false\n");
	return false;
}

bool CRebuyManagement::NoCards(){
	if (!Preferences()->rebuy_condition_no_cards()) {
		return true;
	}
	int UserChair = EngineContainer()->symbol_engine_userchair()->userchair();
	if ((UserChair < 0) || (UserChair > 9)) {
		// "No cards", but not even seated.
		// We should never get into that situation,
		// as the autoplayer can't get enabled without a userchair.
		// If all goes wrong, the rebuy-script has to handle that case.
    write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] No_Cards: true, as userchair not detected\n");
		return true;
	}
	if (!TableState()->User()->HasKnownCards()) 	{
    write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] No_Cards: true.\n");
		return true;
	}
	// We hold cards. No good time to rebuy,
	// if we play at a real casino.
	write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] No_Cards: false: we hold cards.\n");	
	return false;
}

/*#bool CRebuyManagement::OcclusionCheck() {
	if (!Preferences()->rebuy_condition_heuristic_check_for_occlusion()) {
		return true;
	}	else if (occlusion_check.UserBalanceOccluded()) {
		write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] OcclusionCheck: false (occluded)\n");
		return false;
	}
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] OcclusionCheck: true (not occluded)\n");
	return true;
}*/

bool CRebuyManagement::RebuyPossible() {
	if (MinimumDelayElapsed()
      && ChangeInHandNumber()
		  && NoCards()
		  /*#&& OcclusionCheck()*/) {
		write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] RebuyPossible: true\n");
		return true;
	}	else {
		write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] RebuyPossible: false\n");
		return false;
	}
}

bool CRebuyManagement::TryToRebuy() {
	write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] TryToRebuy()\n");
	if (RebuyPossible()) {
		RebuyLastTime = CurrentTime;		
		PreviousRebuyHandNumber = EngineContainer()->HandresetDetector()->GetHandNumber();
		ExecuteRebuyScript();
    return true;
	}
  return false;
}	

void CRebuyManagement::ExecuteRebuyScript() {
	// Call the external rebuy script.
	//
	// CAUTION! DO NOT USE THIS FUNCTION DIRECTLY!
	// It has to be protected by a mutex.
	// We assume, the autoplayer does that.
	//
	// Build command-line-options for rebuy-script
	write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] ExecuteRebuyScript\n");
	CString sitename = BasicScraper()->Tablemap()->sitename();
  if (sitename == "") {
		sitename = "Undefined";
	}
	HWND WindowHandleOfThePokerTable = TableManagement()->AutoConnector()->attached_hwnd();
	double SmallBlind = EngineContainer()->symbol_engine_tablelimits()->sblind();
	double BigBlind = EngineContainer()->symbol_engine_tablelimits()->bblind();
	double BigBet = EngineContainer()->symbol_engine_tablelimits()->bigbet();
	int UserChair = EngineContainer()->symbol_engine_userchair()->userchair();
	double Balance = TableState()->User()->_balance.GetValue();
	double TargetAmount = Formula()->FunctionCollection()->Evaluate(k_standard_function_names[k_hopper_function_rebuy]);
	CString RebuyScript = Preferences()->rebuy_script();
	CString CommandLine;
	CommandLine.Format(CString("%s %s %u %d %.2f %.2f %.2f %.2f %.2f"), 
		RebuyScript, sitename, WindowHandleOfThePokerTable, 
		UserChair, Balance, SmallBlind, BigBlind, BigBet, TargetAmount);
	// For some docu on "CreateProcess" see:
	// http://pheatt.emporia.edu/courses/2005/cs260s05/hand39/hand39.htm
	// http://msdn.microsoft.com/en-us/library/aa908775.aspx
	// http://www.codeproject.com/KB/threads/CreateProcessEx.aspx
	STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo; 
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	memset(&ProcessInfo, 0, sizeof(ProcessInfo));
	StartupInfo.cb = sizeof(StartupInfo); 
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] RebuyScript %s\n", RebuyScript);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Casino %s\n", sitename);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Window %u\n", WindowHandleOfThePokerTable);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Userchair %d\n", UserChair);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Balance %.2f\n", Balance);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Smallblind  %.2f\n", SmallBlind);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Bigblind  %.2f\n", BigBlind);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Bigbet  %.2f\n", BigBet);
  write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] Target-amount  %.2f\n", TargetAmount);
	write_log(Preferences()->debug_rebuy(), "[CRebuyManagement] command line: %s\n", CommandLine);
	if(CreateProcess(NULL, CommandLine.GetBuffer(), NULL, 
		  false, 0, NULL, NULL, 0, &StartupInfo, &ProcessInfo)) {
		// Docu for WaitForSingleObject:
		// http://msdn.microsoft.com/en-us/library/ms687032(VS.85).aspx
		// It seems, none of the exitcodes is relevant for us.
		//
		// Wait for termination of the rebuy-script, if necessary forever,
		// as we can't release the (autoplayer)mutex as long as the script is running.
		int ExitCode = WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	}	else {
		CString ErrorMessage = CString("Could not execute rebuy-script: ") + CString(RebuyScript) + "\n";
		write_log(Preferences()->debug_rebuy(), ErrorMessage.GetBuffer());
		MessageBox_Error_Warning(ErrorMessage.GetBuffer());
	}
}