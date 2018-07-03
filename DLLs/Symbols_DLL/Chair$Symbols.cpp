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

#include "Chair$Symbols.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\Tablestate_DLL\TableState.h"

double Chair$(const char *name)
{
	for (int i=0; i<BasicScraper()->Tablemap()->nchairs(); i++)
	{
		if (TableState()->Player(i)->name().Find(&name[0]) != kNotFound)
			return i;
	}
	return kUndefined;
}

double Chairbit$(const char *name)
{
	int chair = Chair$(name);
	int bits  = 1 << chair;
	return bits;
}