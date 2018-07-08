//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: dealposition, betposition and some other position symbols
//
//******************************************************************************

#include "CSymbolEnginePositions.h"
#include <assert.h>
#include "CEngineContainer.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineRaisers.h"
#include "CSymbolEngineUserchair.h"
#include "..\Globals_DLL\globals.h"
#include "..\Numerical_Functions_DLL\NumericalFunctions.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\Tablestate_DLL\TableState.h"

CSymbolEnginePositions::CSymbolEnginePositions()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(EngineContainer()->symbol_engine_active_dealt_playing() != NULL);
	assert(EngineContainer()->symbol_engine_dealerchair() != NULL);
	assert(EngineContainer()->symbol_engine_userchair() != NULL);
}

CSymbolEnginePositions::~CSymbolEnginePositions()
{}

void CSymbolEnginePositions::InitOnStartup()
{}

void CSymbolEnginePositions::UpdateOnConnection()
{}

void CSymbolEnginePositions::UpdateOnHandreset() {
	_betposition  = 0;
	_dealposition = 0;
	_callposition = 0;
	_nchairsdealtright = 0;
	_nchairsdealtleft  = 0;
	_betpositionrais  = 0;
	_dealpositionrais = 0;
}

void CSymbolEnginePositions::UpdateOnNewRound() {
}

void CSymbolEnginePositions::UpdateOnMyTurn() {
	CalculatePositionForTheRaiser();
	CalculatePositionsForTheUserchair();
}

void CSymbolEnginePositions::UpdateOnHeartbeat() {
	CalculateNChairsDealtLeftRight();
  CalculatePositionsForTheUserchair();
}

void CSymbolEnginePositions::CalculateNChairsDealtLeftRight() {
	_nchairsdealtright = 0;
	_nchairsdealtleft  = 0;

	if (!EngineContainer()->symbol_engine_userchair()->userchair_confirmed())	{
		// Nothing to search for
		return;
	}

	bool found_userchair = false;
	for (int i=DEALER_CHAIR+1; 
		  i<=DEALER_CHAIR+BasicScraper()->Tablemap()->nchairs();
		  i++) {
		int next_chair = i%BasicScraper()->Tablemap()->nchairs();
		double p_bet = TableState()->Player(next_chair)->_bet.GetValue();

		if (next_chair == EngineContainer()->symbol_engine_userchair()->userchair())	{
			found_userchair = true;
		}	else if (IsBitSet(EngineContainer()->symbol_engine_active_dealt_playing()->playersdealtbits(), next_chair)) 	{
			if (!found_userchair)	{
				_nchairsdealtright++;
			}	else {
				_nchairsdealtleft++;
			}
		}
	}
	AssertRange(_nchairsdealtright, 0, (kMaxNumberOfPlayers - 1));
	AssertRange(_nchairsdealtleft,  0, (kMaxNumberOfPlayers - 1));
}

void CSymbolEnginePositions::CalculatePositionForTheRaiser() {
	_betpositionrais  = 0;
	_dealpositionrais = 0;

	for (int i=DEALER_CHAIR+1; 
		  i<=(DEALER_CHAIR+BasicScraper()->Tablemap()->nchairs());
		  i++) {
		int next_chair = i%BasicScraper()->Tablemap()->nchairs();
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=20746
		if (IsBitSet(EngineContainer()->symbol_engine_active_dealt_playing()->playersplayingbits(), next_chair)) {
			_betpositionrais++;
		}
    if (IsBitSet(EngineContainer()->symbol_engine_active_dealt_playing()->playersdealtbits(), next_chair)) {
			_dealpositionrais++;
		}
    if (next_chair == EngineContainer()->symbol_engine_raisers()->raischair()) {
      break;
    }
	}
	AssertRange(_betpositionrais,  kUndefined, kMaxNumberOfPlayers);
	AssertRange(_dealpositionrais, kUndefined, kMaxNumberOfPlayers);
}

void CSymbolEnginePositions::CalculatePositionsForTheUserchair() {
	_betposition  = 0;
	_dealposition = 0;
	_callposition = 0;

	for (int i=DEALER_CHAIR+1; 
		  i<=DEALER_CHAIR+BasicScraper()->Tablemap()->nchairs();
		  i++) {
		int next_chair = i%BasicScraper()->Tablemap()->nchairs();
		if (IsBitSet(EngineContainer()->symbol_engine_active_dealt_playing()->playersplayingbits(), next_chair))	{
			_betposition++;
		}
		if (IsBitSet(EngineContainer()->symbol_engine_active_dealt_playing()->playersdealtbits(), next_chair)) {
			_dealposition++;
		}
		if ((next_chair) == EngineContainer()->symbol_engine_userchair()->userchair())	{
			// Stop searching
			break;
		}
	}

	int raischair = EngineContainer()->symbol_engine_raisers()->raischair();
	for (int i=raischair+1; i<=raischair+BasicScraper()->Tablemap()->nchairs(); i++) 	{
		int next_chair = i%BasicScraper()->Tablemap()->nchairs();
		if (IsBitSet(EngineContainer()->symbol_engine_active_dealt_playing()->nplayersdealt(), next_chair)) 	{
			_callposition++;
		}
	}

  // calculate _callposition; _betpositionrais must have been calculated at this point
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=20746
  int nplayers = EngineContainer()->symbol_engine_active_dealt_playing()->nplayersplaying();
  int offset = (_betposition + nplayers - _betpositionrais);
  if (nplayers > 0) {
    _callposition = offset % nplayers;
  }

	AssertRange(_betposition,  kUndefined, kMaxNumberOfPlayers);
	AssertRange(_dealposition, kUndefined, kMaxNumberOfPlayers);
	AssertRange(_callposition, kUndefined, kMaxNumberOfPlayers);
}

bool CSymbolEnginePositions::EvaluateSymbol(const CString name, double *result, bool log /* = false */)
{
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "nchairsdealt", 12)==0)
	{
		if (memcmp(name, "nchairsdealtright", 17)==0 && strlen(name)==17)
		{
			*result = nchairsdealtright();
		}
		else if (memcmp(name, "nchairsdealtleft", 16)==0 && strlen(name)==16)
		{
			*result = nchairsdealtleft();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "betposition", 11)==0)
	{
		if (memcmp(name, "betposition", 11)==0 && strlen(name)==11)		
		{
			*result = betposition();
		}
		else if (memcmp(name, "betpositionrais", 15)==0 && strlen(name)==15)	
		{
			*result = betpositionrais();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "dealposition", 12)==0)
	{
		if (memcmp(name, "dealposition", 12)==0 && strlen(name)==12)	
		{
			*result = dealposition();
		}
		else if (memcmp(name, "dealpositionrais", 16)==0 && strlen(name)==16)
		{
			*result = dealpositionrais();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "callposition", 12)==0 && strlen(name)==12)	
	{
		*result = callposition();
		// Valid symbol
		return true;
	}
	else
	{
		// Symbol of a different symbol-engine
		return false;
	}
}

CString CSymbolEnginePositions::SymbolsProvided() {
  return "nchairsdealt nchairsdealtright nchairsdealtleft "
    "betposition betpositionrais "
    "dealposition dealpositionrais "
    "callposition ";
}