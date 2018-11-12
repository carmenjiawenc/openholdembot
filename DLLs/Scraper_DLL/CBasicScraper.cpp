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
//   * NumericalFunctions.DLL
//   * StringFunctions.DLL
//   * WindowFunction.DLL
// These DLLs are expected to be in the same directory.
//
//*******************************************************************************

#define SCRAPER_DLL_EXPORTS

#include "CBasicScraper.h"
#include "CTablemap\CTablemapCompletenessChecker.h"

// Singleton for CBasicScraper
// Gets initialized via the accessor-function once needed
CBasicScraper* p_basic_scraper = NULL;

CBasicScraper* BasicScraper() {
  if (p_basic_scraper == NULL) {
    p_basic_scraper = new CBasicScraper();
  }
  assert(p_basic_scraper != NULL);
  return p_basic_scraper;
}

CBasicScraper::CBasicScraper() {
}

CBasicScraper::~CBasicScraper(){
}

bool CBasicScraper::LoadTablemap(const char* path) {
  bool success = _tablemap.LoadTablemap(path);
  if (!success) {
    return false;
  }
  // Map will be verified as long as it is not marked as popup or lobby
  CTablemapCompletenessChecker tablemap_completeness_checker;
  tablemap_completeness_checker.VerifyMap();
  return true;
}

CString CBasicScraper::ScrapeRegion(const CString name) {
  //!!!!!!!
  /*#
  __HDC_HEADER
  write_log(Preferences()->debug_scraper(),
    "[CScraper] EvaluateRegion %s\n", name);
	CTransform	trans;
	RMapCI		r_iter = BasicScraper()->Tablemap()->r$()->find(name.GetString());
	if (r_iter != BasicScraper()->Tablemap()->r$()->end()) {
    // Potential for optimization here
    ++total_region_counter;
		if (ProcessRegion(r_iter)) {
      ++identical_region_counter;
      write_log(Preferences()->debug_scraper(),
        "[CScraper] Region %s identical\n", name);
    } else {
      write_log(Preferences()->debug_scraper(),
        "[CScraper] Region %s NOT identical\n", name);
    }
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
		trans.DoTransform(r_iter, hdcCompatible, result);
		SelectObject(hdcCompatible, old_bitmap);
		write_log(Preferences()->debug_scraper(), "[CScraper] EvaluateRegion(), [%s] -> [%s]\n", 
			name, *result);
    __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
		return true;
	}
	// Region does not exist
  *result = "";
	__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
	return false;*/
  return "error";
}

//*******************************************************************************
//
// Simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************

HWND scraped_window = NULL;

// Assigns a table to the scraper
// The order of ConnectScraperToWindow and LoadTablemap
// does not matter, but both of them must have been called
// before ScrapeRegion.
SIMPLE_SCRAPER_DLL_API void ConnectScraperToWindow(HWND window) {
  scraped_window = window;
}

// Loads a tablemap (and automatically unloads the previous one)
SIMPLE_SCRAPER_DLL_API bool LoadTablemap(const char* path) {
  return BasicScraper()->LoadTablemap(path);
}

// result-buffer has to be managed by the caller
// returned results are usually numbers and player-names,
// so any reasonable buffer should do OK, we recommend 255 ybtes.
SIMPLE_SCRAPER_DLL_API void ScrapeRegion(const char* in_name, const int in_result_buffer_size, char* out_result) {
  if (out_result == NULL) {
    // Invalid buffer, do nothing
    return;
  } 
  if (in_result_buffer_size <= 0) {
    // Invalid buffer, do nothing
    return;
  } 
  CString result = BasicScraper()->ScrapeRegion(in_name);
  if (result.GetLength() >= in_result_buffer_size){
    // Buffer too small for result + terminal null-character
	  // Clear buffer and return
    out_result = '\0';
    return;
  }
  memcpy(out_result, result.GetBuffer(), (result.GetLength() + 1));
}