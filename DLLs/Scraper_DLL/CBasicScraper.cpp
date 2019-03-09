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
#include "CTransform\CTransform.h"
#include "..\Bitmaps_DLL\Bitmaps.h"

// !!!! take care of potential memory-leaks

// Singleton for CBasicScraper
// Gets initialized via the accessor-function once needed
CBasicScraper* p_basic_scraper = NULL;

#define __HDC_HEADER 		HBITMAP		old_bitmap = NULL; \
	HDC				hdc = GetDC(_connected_window); \
	HDC				hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); \
	HDC				hdcCompatible = CreateCompatibleDC(hdcScreen); \
  ++_leaking_GDI_objects;

#define __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK \
  DeleteDC(hdcCompatible); \
	DeleteDC(hdcScreen); \
	ReleaseDC(_connected_window, hdc); \
  --_leaking_GDI_objects;

CBasicScraper* BasicScraper() {
  if (p_basic_scraper == NULL) {
    p_basic_scraper = new CBasicScraper();
  }
  assert(p_basic_scraper != NULL);
  return p_basic_scraper;
}

CBasicScraper::CBasicScraper() {
  _connected_window = NULL;
  _leaking_GDI_objects = 0;
  _total_region_counter = 0;
  _identical_region_counter = 0;
}

CBasicScraper::~CBasicScraper() {
  /*#if (_leaking_GDI_objects != 0) {
    write_log(k_always_log_errors, "[CScraper] ERROR! Leaking GDI objects: %i\n",
      _leaking_GDI_objects);
    write_log(k_always_log_errors, "[CScraper] Please get in contact with the development team\n");
  }
  write_log(true, "[CScraper] Total regions scraped %i\n",
    _total_region_counter);
  write_log(true, "[CScraper] Identical regions scraped %i\n",
    _identical_region_counter);*/
}

bool CBasicScraper::LoadTablemap(const char* path) {
  //!!!!! bool success is WRONG, why no warning?
  int success = _tablemap.LoadTablemap(path);
  if (success != SUCCESS) {
    MessageBox(0, "Fail", "LoadTablemap", 0);
    return false;
  }
  // Map will be verified as long as it is not marked as popup or lobby
  CTablemapCompletenessChecker tablemap_completeness_checker;
  tablemap_completeness_checker.VerifyMap();
  MessageBox(0, "Success", "LoadTablemap", 0);
  return true;
}

void CBasicScraper::ConnectScraperToWindow(HWND window) {
  _connected_window = window;
}

void CBasicScraper::CreateBitmaps(void) {
  HDC	hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
  // Whole window
  RECT cr = { 0 };
  GetClientRect(_connected_window, &cr);
  _entire_window_last = CreateCompatibleBitmap(hdcScreen, cr.right, cr.bottom);
  _entire_window_cur = CreateCompatibleBitmap(hdcScreen, cr.right, cr.bottom);
  // r$regions
  for (RMapI r_iter = BasicScraper()->Tablemap()->set_r$()->begin(); r_iter != BasicScraper()->Tablemap()->set_r$()->end(); r_iter++) {
    int w = r_iter->second.right - r_iter->second.left + 1;
    int h = r_iter->second.bottom - r_iter->second.top + 1;
    r_iter->second.last_bmp = CreateCompatibleBitmap(hdcScreen, w, h);
    r_iter->second.cur_bmp = CreateCompatibleBitmap(hdcScreen, w, h);
  }
  DeleteDC(hdcScreen);
}

void CBasicScraper::DeleteBitmaps(void) {
  // Whole window
  DeleteObject(_entire_window_last);
  delete_entire_window_cur();
  // Common cards
  for (RMapI r_iter = BasicScraper()->Tablemap()->set_r$()->begin(); r_iter != BasicScraper()->Tablemap()->set_r$()->end(); r_iter++) {
    DeleteObject(r_iter->second.last_bmp); r_iter->second.last_bmp = NULL;
    DeleteObject(r_iter->second.cur_bmp); r_iter->second.cur_bmp = NULL;
  }
}

void CBasicScraper::delete_entire_window_cur() { 
  ///CSLock lock(m_critsec);
  DeleteObject(_entire_window_cur); 
}

bool CBasicScraper::IsIdenticalScrape() {
  __HDC_HEADER
  // Get bitmap of whole window
  RECT		cr = { 0 };
  GetClientRect(_connected_window, &cr);
  old_bitmap = (HBITMAP)SelectObject(hdcCompatible, _entire_window_cur);
  BitBlt(hdcCompatible, 0, 0, cr.right, cr.bottom, hdc, cr.left, cr.top, SRCCOPY);
  SelectObject(hdcCompatible, old_bitmap);
  // If the bitmaps are the same, then return now
  // !! How often does this happen?
  // !! How costly is the comparison?
  if (BitmapsAreEqual(_entire_window_last, _entire_window_cur)) {
    DeleteDC(hdcCompatible);
    DeleteDC(hdcScreen);
    ReleaseDC(_connected_window, hdc);
    ///write_log(Preferences()->debug_scraper(), "[CScraper] IsIdenticalScrape() true\n");
    __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
    return true;
  }
  // Copy into "last" bitmap
  old_bitmap = (HBITMAP)SelectObject(hdcCompatible, _entire_window_last);
  BitBlt(hdcCompatible, 0, 0, cr.right - cr.left + 1, cr.bottom - cr.top + 1, hdc, cr.left, cr.top, SRCCOPY);
  SelectObject(hdc, old_bitmap);
  __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
    ///write_log(Preferences()->debug_scraper(), "[CScraper] IsIdenticalScrape() false\n");
  return false;
}

CString CBasicScraper::ScrapeRegion(const CString name) {
  //!!! delete last
  _entire_window_last = _entire_window_cur;
  TakeScreenshot(_connected_window, &_entire_window_cur);
  SaveBitmapToFile(_entire_window_cur, "debug2.bmp");
  __HDC_HEADER
  /*#write_log(Preferences()->debug_scraper(),
    "[CScraper] EvaluateRegion %s\n", name);*/
	CTransform	trans;
	RMapCI		  r_iter = BasicScraper()->Tablemap()->r$()->find(name.GetString());
  CString result = "";
  MessageBox(0, name, "Scraping Region", 0);
  if (r_iter == Tablemap()->r$()->end()) {
    // Region not found
    MessageBox(0, "Region not found", "ScrapeRegion", 0);
  } else {
    MessageBox(0, "Region found", "ScrapeRegion", 0);
    // Potential for optimization here
    ++_total_region_counter;
		if (ProcessRegion(r_iter)) {
      ++_identical_region_counter;
      /*#write_log(Preferences()->debug_scraper(),
        "[CScraper] Region %s identical\n", name);*/
    } else {
      /*#write_log(Preferences()->debug_scraper(),
        "[CScraper] Region %s NOT identical\n", name);*/
    }
    MessageBox(0, "Going to transform", "Scraping Region", 0);
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
    SaveBitmapToFile(_entire_window_cur, "debug3_old.bmp");
    if (r_iter->second.cur_bmp == NULL) {
      MessageBox(0, "r_iter->second.cur_bmp is NULL", "Error", 0);
    }
    SaveBitmapToFile(r_iter->second.cur_bmp, "debug4_iter_cur.bmp");
		trans.DoTransform(r_iter, hdcCompatible, &result);
		SelectObject(hdcCompatible, old_bitmap);
	}
  /*#write_log(Preferences()->debug_scraper(), "[CScraper] EvaluateRegion(), [%s] -> [%s]\n",
  name, *result);*/
	__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
  return result;
}

bool CBasicScraper::ProcessRegion(RMapCI r_iter) {
  MessageBox(0, r_iter->first, "ProcessRegion", 0);
  /*#write_log(Preferences()->debug_scraper(),
    "[CScraper] ProcessRegion %s (%i, %i, %i, %i)\n",
    r_iter->first, r_iter->second.left, r_iter->second.top,
    r_iter->second.right, r_iter->second.bottom);
  write_log(Preferences()->debug_scraper(),
    "[CScraper] ProcessRegion color %i radius %i transform %s\n",
    r_iter->second.color, r_iter->second.radius, r_iter->second.transform);
    */
  __HDC_HEADER
    // Get "current" bitmap
    old_bitmap = (HBITMAP)SelectObject(hdcCompatible, r_iter->second.cur_bmp);
    BitBlt(hdcCompatible, 0, 0, r_iter->second.right - r_iter->second.left + 1,
      r_iter->second.bottom - r_iter->second.top + 1,
      hdc, r_iter->second.left, r_iter->second.top, SRCCOPY);
    SelectObject(hdcCompatible, old_bitmap);
    // If the bitmaps are different, then continue on
    if (!BitmapsAreEqual(r_iter->second.last_bmp, r_iter->second.cur_bmp)) {
      MessageBox(0, "Bitmaps not equal", "ProcessRegion", 0);
      // Copy into "last" bitmap
      old_bitmap = (HBITMAP)SelectObject(hdcCompatible, r_iter->second.last_bmp);
      BitBlt(hdcCompatible, 0, 0, r_iter->second.right - r_iter->second.left + 1,
        r_iter->second.bottom - r_iter->second.top + 1,
        hdc, r_iter->second.left, r_iter->second.top, SRCCOPY);
      SelectObject(hdcCompatible, old_bitmap);
    __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
    return true;
  }
    MessageBox(0, "Bitmaps equal", "ProcessRegion", 0);
  __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
  return false;
}


//*******************************************************************************
//
// Simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************

// Assigns a table to the scraper
// The order of ConnectScraperToWindow and LoadTablemap
// does not matter, but both of them must have been called
// before ScrapeRegion.
SIMPLE_SCRAPER_DLL_API void ConnectScraperToWindow(HWND window) {
  BasicScraper()->ConnectScraperToWindow(window);
}

// Loads a tablemap (and automatically unloads the previous one)
SIMPLE_SCRAPER_DLL_API bool LoadTablemap(const char* path) {
  MessageBox(0, path, "LoadTablemap", 0);
  return BasicScraper()->LoadTablemap(path);
}

/*!!!// Takes a screenshot of the connected window / table.
// Thereafter ScrapeRegion can be called.
SIMPLE_SCRAPER_DLL_API void TakeScreenshot() {
  MessageBox(0, "TakeScreenshot", "TakeScreenshot", 0);
  BasicScraper()->TakeScreenshot();
}*/

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
  //!!!!!
  MessageBox(0, result, "ScrapeRegion Result", 0);
}
