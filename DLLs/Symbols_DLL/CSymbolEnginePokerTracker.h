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
// Purpose:
//
//******************************************************************************

#include "CVirtualSymbolEngine.h"

class CPokerTrackerThread;

class CSymbolEnginePokerTracker: public CVirtualSymbolEngine {
 public:
	CSymbolEnginePokerTracker();
	~CSymbolEnginePokerTracker();
 public:
	// Mandatory reset-functions
	void InitOnStartup();
	void UpdateOnConnection();
	void UpdateOnHandreset();
	void UpdateOnNewRound();
	void UpdateOnMyTurn();
	void UpdateOnHeartbeat();
 public:
	// Public accessors	
	bool EvaluateSymbol(const CString name, double *result, bool log = false);
	CString SymbolsProvided();
 private:
	void	CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup();
	void	WarnAboutInvalidPTSymbol(CString s);
	bool	IsOldStylePTSymbol(CString s);
	void	ClearSeatStats(int chair, bool clearNameAndFound);
	void	ClearAllStatsOfChangedPlayers();
	void	ClearAllStats();
 private:
	bool  check_for_identity_of_players_executed_this_heartbeat;
  CPokerTrackerThread* _p_poker_tracker_thread;
};