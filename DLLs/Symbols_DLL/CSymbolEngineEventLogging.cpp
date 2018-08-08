//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Not really a symbol-engine, 
//   but a good way to get important events logged.
//
//******************************************************************************

#include "CSymbolEngineEventLogging.h"
#include "CEngineContainer.h"
#include "CBetroundCalculator.h"
#include "CHandresetDetector.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEngineUserchair.h"
#include "CVersionInfo.h"
#include "..\Debug_DLL\debug.h"
#include "..\Formula_DLL\CAutoplayerTrace.h"
#include "..\Formula_DLL\CFormula.h"
#include "..\Formula_DLL\CFunctionCollection.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Tablestate_DLL\TableState.h"
#include "..\Tablestate_DLL\CTableTitle.h"
#include "..\..\OpenHoldem_old\stdafx.h"

CSymbolEngineEventLogging::CSymbolEngineEventLogging()
{}

CSymbolEngineEventLogging::~CSymbolEngineEventLogging()
{}

void CSymbolEngineEventLogging::InitOnStartup()
{}

void CSymbolEngineEventLogging::UpdateOnConnection() {
  Formula()->AutoplayerTrace()->Clear();
}

void CSymbolEngineEventLogging::UpdateOnHandreset() {
  // Log a new connection, plus the version-info
  // (because of all the guys who report "bugs" of outdated versions)
  write_log(k_always_log_basic_information,
    "\n"
    "==============================================\n"
    "%s"    // Version info already contains a newline
    "Table: %s\n"
    "==============================================\n"
    "HAND RESET\n"
    "==============================================\n",
    "", version_info->GetVersionInfo(), 
    TableState()->TableTitle()->Title());
}

void CSymbolEngineEventLogging::UpdateOnNewRound()
{}

void CSymbolEngineEventLogging::UpdateOnMyTurn()
{}

void CSymbolEngineEventLogging::UpdateOnHeartbeat() {
  //#Formula()->AutoplayerTrace()->Clear();
}

void CSymbolEngineEventLogging::UpdateAfterAutoplayerAction(int autoplayer_action_code) {
  LogPlayers(); 
  LogBasicInfo("DUMMY"); ///
  ///Trace
}

bool CSymbolEngineEventLogging::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
	// We don't provide any symbols
	return false;
}

void CSymbolEngineEventLogging::LogBasicInfo(const char *action_taken) {
  CString	comcards, temp, rank, pokerhand;
  CString	fcra_formula_status;
  int		userchair = EngineContainer()->symbol_engine_userchair()->userchair();
  int		betround  = EngineContainer()->BetroundCalculator()->betround();
  // Player cards
  // There always exists a user, if not then we have a fake-player. ;-)
  assert(TableState()->User() != NULL);
  CString player_cards = TableState()->User()->Cards();
  // Common cards
  comcards = "";
  if (betround >= kBetroundFlop) {
    for (int i=0; i<kNumberOfFlopCards; i++) {
      if (TableState()->CommonCards(i)->IsKnownCard()) {
        comcards.Append(TableState()->CommonCards(i)->ToString());
      }
    }
  }
  if (betround >= kBetroundTurn) {
    comcards.Append(TableState()->TurnCard()->ToString());
  }
  if (betround >= kBetroundRiver) {
    comcards.Append(TableState()->RiverCard()->ToString());
  }
  comcards.Append("..........");
  comcards = comcards.Left(10);
  // Always use handrank169 here
  rank.Format("%.0f", EngineContainer()->symbol_engine_handrank()->handrank169());
  // poker hand
  pokerhand = EngineContainer()->symbol_engine_pokerval()->HandType();
  // fcra_seen
  CString fcra_seen = EngineContainer()->symbol_engine_autoplayer()->GetFCKRAString();
  // fcra formula status
  fcra_formula_status.Format("%s%s%s%s%s",
  Formula()->FunctionCollection()->EvaluateAutoplayerFunction(k_autoplayer_function_fold)  ? "F" : ".",
  Formula()->FunctionCollection()->EvaluateAutoplayerFunction(k_autoplayer_function_call)  ? "C" : ".",
  Formula()->FunctionCollection()->EvaluateAutoplayerFunction(k_autoplayer_function_check) ? "K" : ".",
  Formula()->FunctionCollection()->EvaluateAutoplayerFunction(k_autoplayer_function_raise) ? "R" : ".",
  Formula()->FunctionCollection()->EvaluateAutoplayerFunction(k_autoplayer_function_allin) ? "A" : ".");
  // More verbose summary in the log
  // The old WinHoldem format was a complete mess
  write_log_separator(k_always_log_basic_information, "Basic Info");
  write_log(k_always_log_basic_information, "  Version:       %s\n",    VERSION_TEXT);
  write_log(k_always_log_basic_information, "  Handnumber:    %s\n",    EngineContainer()->HandresetDetector()->GetHandNumber());
  write_log(k_always_log_basic_information, "  Chairs:        %5d\n",   BasicScraper()->Tablemap()->nchairs());
  write_log(k_always_log_basic_information, "  Userchair:     %5d\n",   userchair);
  write_log(k_always_log_basic_information, "  Holecards:     %s\n",    player_cards.GetString());
  write_log(k_always_log_basic_information, "  Community:     %s\n",    comcards.GetString());
  write_log(k_always_log_basic_information, "  Handrank:      %s\n",    rank.GetString());
  write_log(k_always_log_basic_information, "  Hand:          %s\n",    pokerhand.GetString());
  write_log(k_always_log_basic_information, "  My balance:    %9.2f\n", TableState()->User()->_balance.GetValue());
  write_log(k_always_log_basic_information, "  My currentbet: %9.2f\n", TableState()->User()->_bet.GetValue());
  write_log(k_always_log_basic_information, "  To call:       %9.2f\n", EngineContainer()->symbol_engine_chip_amounts()->call());
  write_log(k_always_log_basic_information, "  Pot:           %9.2f\n", EngineContainer()->symbol_engine_chip_amounts()->pot());
  write_log(k_always_log_basic_information, "  Big blind:     %9.2f\n", EngineContainer()->symbol_engine_tablelimits()->bblind());
  write_log(k_always_log_basic_information, "  Big bet (FL):  %9.2f\n", EngineContainer()->symbol_engine_tablelimits()->bigbet());
  write_log(k_always_log_basic_information, "  f$betsize:     %9.2f\n", Formula()->FunctionCollection()->EvaluateAutoplayerFunction(k_autoplayer_function_betsize));
  write_log(k_always_log_basic_information, "  Formulas:      %s\n",    fcra_formula_status.GetString());
  write_log(k_always_log_basic_information, "  Buttons:       %s\n",    fcra_seen.GetString());
  // !! "Best action" is undefined if the executed action
  // is "something else" like a hopper function
  ///write_log(k_always_log_basic_information, "  Best action:   %s\n", BestAction().GetString());
  write_log(k_always_log_basic_information, "  Action taken:  %s\n",    action_taken);
  write_log_separator(k_always_log_basic_information, "");
}

void CSymbolEngineEventLogging::LogPlayers() {
  write_log_separator(k_always_log_basic_information, "Players");
  // Logging all players at the table
  // starting at userchair (hero), so that we can easily see all raises behind him
  int	userchair = EngineContainer()->symbol_engine_userchair()->userchair();
  int nchairs = BasicScraper()->Tablemap()->nchairs();
  for (int i = 0; i < nchairs; ++i) {
    int chair = (userchair + i) % nchairs;
    CString data;
    data.Format("Chair %2i  %s\n", chair, TableState()->Player(chair)->DataDump());
   write_log(k_always_log_basic_information, data.GetBuffer());
  }
  write_log_separator(k_always_log_basic_information, "");
}