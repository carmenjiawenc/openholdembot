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

#include "CLazyScraper.h"
#include <assert.h>
#include "..\CasinoInterface_DLL\CCasinoInterface.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CHandresetDetector.h"
#include "..\Symbols_DLL\CSymbolEngineActiveDealtPlaying.h"
#include "..\Symbols_DLL\CSymbolEngineGameType.h"
#include "..\Symbols_DLL\CSymbolEngineHistory.h"
#include "..\Symbols_DLL\CSymbolengineTime.h"
#include "..\Symbols_DLL\CSymbolengineUserchair.h"
#include "..\TableState_DLL\TableState.h"

CLazyScraper::CLazyScraper() {
  _is_identical_scrape = false;
}

CLazyScraper::~CLazyScraper() {
}

// Some ideas:
// ===========
//
// Things we need to scrape every heartbeat,
// depending on handreset-method:
//   * handnumber
//   * action-buttons (standard, no betpot)
//   * cards of the userchair
//
// Things we need to scrape if the userchair is not yet known:
//   * action-buttons (standard, no betpot)
//   * all players cards
//
// Things we need to scrape on our turn
//   * betpot-buttons
//   * slider
// (only if the game-type is NL or PL)
//
// Once per every heartbeat (cash-game):
//   * scrape "seated" for all chairs
//   * scrape "active" for all seated chairs
//   * scrape cards for all active chairs
//   * scrape bets and balances for all active chairs
// (but we might skip this if we have folded 
// and the hand-history-generator is disabled)
//
// Tournaments:
//   * scrape bets and balances for all chairs when our turn because of ICM
//
// Name-scraping
// Preflop up to our first action only, because:
//   * once it is our turn we have stable frames
//   * later name-changes (new players) don't affect the game
//   * potential occlusion later can no longer affect PT
//   * improved CPU-usage as name-scraping needs about 40%
//
// DLLs and/or HH-gen might need all data all the time.
// If in doubt be conservative.

void CLazyScraper::DoScrape() {
  ///CTableTitle()->UpdateTitle();
  ///if identical title
	if (scraper.IsIdenticalScrape())	{
		_is_identical_scrape = true;
    return;
	}
  _is_identical_scrape = false;
	scraper.ScrapeLimits();
	if (NeedDealerChair()) { 
		scraper.ScrapeDealer();
	}
	if (NeedUsersCards())	{
		assert(EngineContainer()->symbol_engine_userchair()->userchair_confirmed());
		scraper.ScrapePlayerCards(EngineContainer()->symbol_engine_userchair()->userchair());
	}
	scraper.ScrapeSeatedActive();
	if (NeedAllPlayersCards()) {
		scraper.ScrapeAllPlayerCards(); 
	}
	if (NeedCommunityCards())	{
		scraper.ScrapeCommonCards();
	}
	if (NeedFoldButton())	{
		// For fast detection of my turn
		// Currently included in NeedActionbuttons()
    // No extra-scrape of fold-button for improved reaction time
	}
	if (NeedActionbuttons()) {
		scraper.ScrapeActionButtons();
		scraper.ScrapeActionButtonLabels();
	}
	if (NeedInterfaceButtons())	{
		scraper.ScrapeInterfaceButtons();
	}
	if (NeedBetpotButtons()) {
		scraper.ScrapeBetpotButtons();
	}
	if (NeedSlider())	{
		scraper.ScrapeSlider();
	}
	// Swagbox AKA i3edit does not need to be scraped
	// The CasinoInterface checks the existence and uses this region automatically
	if (NeedBetsAndBalances()) {
		scraper.ScrapeBetsAndBalances();
		scraper.ScrapePots();
	}
	if (NeedAllPlayerNames())	{
		scraper.ClearAllPlayerNames();
		ScrapeUnknownPlayerNames();
	}
	if (NeedUnknownPlayerNames())	{
		ScrapeUnknownPlayerNames();
	}
  if (NeedColourCodes()) {
    scraper.ScrapeColourCodes();
  }
  if (NeedMTTRegions()) {
    scraper.ScrapeMTTRegions();
  }
}

bool CLazyScraper::NeedDealerChair() {
	return true;
}

bool CLazyScraper::NeedHandNumber() {
	return true;
}

bool CLazyScraper::NeedUsersCards() {
	return (EngineContainer()->symbol_engine_userchair()->userchair_confirmed());
}

bool CLazyScraper::NeedAllPlayersCards() {
	// User playing
	// HH-gen
	// DLL?
	return true;
}

bool CLazyScraper::NeedFoldButton() {
	// To detect my turn
	return true;
}

bool CLazyScraper::NeedActionbuttons() {
	// If MyTurn
	return true;
}

bool CLazyScraper::NeedInterfaceButtons() {
	return true;
}

bool CLazyScraper::NeedBetpotButtons() {
	return (CasinoInterface()->IsMyTurn()
		&& (EngineContainer()->symbol_engine_gametype()->isnl() || EngineContainer()->symbol_engine_gametype()->ispl()));
}

bool CLazyScraper::NeedSlider() {
	return NeedBetpotButtons(); 
}

bool CLazyScraper::NeedBetsAndBalances() {
	return true;
}

bool CLazyScraper::NeedAllPlayerNames() {
	// Scraping names is the most costly part and mostly needed for PokerTracker.
	// It is enough if we do this until our turn, because
	// * at our turn we have stable frames
	// * new players after our turn can't affect the current hand
  if (EngineContainer()->symbol_engine_history()->DidActThisHand()) {
    return false;
  }
  // We can also stop scraping names if we see new cards 
  // after a hand-reset because then a poterntial new player
  // can no longer join the game.
  if ((EngineContainer()->symbol_engine_time()->elapsedhand() > 2)
      && (EngineContainer()->symbol_engine_active_dealt_playing()->nplayersdealt() >= 2)) {
    return false;
  }
  return true;
}

bool CLazyScraper::NeedUnknownPlayerNames() {
	// Always true
	// If a new player sits down or we missed an existing player
	// due to occlusion we should always scrape that name.
	return true;
}

bool CLazyScraper::NeedCommunityCards() {
	return true;
}

void CLazyScraper::ScrapeUnknownPlayerNames() {
	for (int i=0; i<BasicScraper()->Tablemap()->nchairs(); i++) {
		if (TableState()->Player(i)->seated()
			  && (TableState()->Player(i)->name() == "")) {
			scraper.ScrapeName(i);
		}
	}
}

bool CLazyScraper::NeedColourCodes() {
  // Scrape colour-codes at the beginning of a session 
  // and at my turn -- that's enough.
  return (CasinoInterface()->IsMyTurn()
    || (EngineContainer()->HandresetDetector()->hands_played() <= 1));
}

bool CLazyScraper::NeedMTTRegions() {
  // return when it is our turn
  // or if we have played less than 3 hands (for possible mtt detect)
  return (CasinoInterface()->IsMyTurn()
	  || EngineContainer()->HandresetDetector()->hands_played() < 3);
}