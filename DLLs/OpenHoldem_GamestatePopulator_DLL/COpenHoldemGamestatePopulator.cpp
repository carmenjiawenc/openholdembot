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
// Purpose: Collecting info from the casino
//   and populating the gamestate.class,
//   usually done by scraping.
//
//******************************************************************************

#define GAMESTATE_POPULATOR_DLL_EXPORTS

#include "COpenHoldemGamestatePopulator.h"
#include "CLazyScraper.h"
#include "..\TableState_DLL\TableState.h"

COpenHoldemGamestatePopulator::COpenHoldemGamestatePopulator() {
  lazy_scraper = new CLazyScraper;
}

COpenHoldemGamestatePopulator::~COpenHoldemGamestatePopulator() {
  /// !!! delete objects
}

void COpenHoldemGamestatePopulator::Populate() {
  TableState()->TableTitle()->UpdateTitle();
  lazy_scraper->DoScrape();
}
