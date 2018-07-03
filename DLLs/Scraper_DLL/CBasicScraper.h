#pragma once
//*******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Two interfaces for OpenHoldems scraper-DLL
//.  * object-oriented interface for OpenHoldem.
//   * simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************
//
// The scraper.DLL depends on
//   * numerical_functions.DLL
//   * string_functions.DLL
//   * window_function.DLL
// These DLLs are expected to be in the same directory.
//
//*******************************************************************************

#include <afxwin.h>
#include "LibDef.h"
#include "CTablemap\CTablemap.h"
#include "CTablemap\CTableMapAccess.h"

//*******************************************************************************
//
// Object-oriented interface for OpenHoldem.
//
//*******************************************************************************

class CBasicScraper;

// Accessor for the singleton of CBasicScraper
// Singleton gets initialized via the accessor-function once needed
SCRAPER_DLL_API CBasicScraper* BasicScraper();

class SCRAPER_DLL_API CBasicScraper {
public:
  CBasicScraper();
  ~CBasicScraper();
public:
  bool LoadTablemap(const char* path);
  // Loads a tablemap (and automatically unloads the previous one)
  CString ScrapeRegion(const CString name);
  CTablemap* Tablemap() { return &_tablemap; }
  CTablemapAccess* TablemapAccess() { return &_tablemap_access; }
private:
  CTablemap _tablemap;
  CTablemapAccess _tablemap_access;
};

//*******************************************************************************
//
// Simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************

// Loads a tablemap (and automatically unloads the previous one)
SCRAPER_DLL_API bool LoadTablemap(const char* path);

// result-buffer has to be managed by the caller
// returned results are usually numbers and player-names,
// so any reasonable buffer should do OK, we recommend 255 ybtes.
SCRAPER_DLL_API void ScrapeRegion(const char* in_name, const int in_result_buffer_size, char* out_result);