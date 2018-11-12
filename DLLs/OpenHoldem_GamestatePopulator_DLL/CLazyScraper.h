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

#include "CScraper.h"
#include "..\MemoryManagement_DLL\CSpaceOptimizedGlobalObject.h"

class CLazyScraper: public CSpaceOptimizedGlobalObject {
 public:
	CLazyScraper();
	~CLazyScraper();
 public:
	void DoScrape();
  // This function should be preferred
  // and not CScraper::IsIdenticalScrape()
  // because this one just caches the function result.
  bool IsIdenticalScrape()  { return _is_identical_scrape; }
 private:
	bool NeedDealerChair();
	bool NeedHandNumber();
	bool NeedUsersCards();
	bool NeedAllPlayersCards();
	bool NeedFoldButton();
	bool NeedActionbuttons();
	bool NeedInterfaceButtons();
	bool NeedBetpotButtons();
	bool NeedSlider();
	bool NeedBetsAndBalances();
	bool NeedAllPlayerNames();
	bool NeedUnknownPlayerNames();
	bool NeedCommunityCards();
  bool NeedColourCodes();
  bool NeedMTTRegions();
 private:
	void ScrapeUnknownPlayerNames();
 private:
	bool CardScrapeNeeded();
	bool CompleteScrapeNeeded();
 private:
   CScraper scraper;
  bool _is_identical_scrape;
};