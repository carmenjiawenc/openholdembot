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
// Purpose: Scraping the poker-table and providing access to the scraped data.
//  As the CScraper is low-level and quite large we created 
//  a interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//******************************************************************************

#include <afxwin.h>
#include "LibDef.h"

BITMAPS_API bool BitmapsAreEqual(HBITMAP HBitmapLeft, HBITMAP HBitmapRight) ;
BITMAPS_API void SaveBitmapToFile(HBITMAP bitmap, char* path);
BITMAPS_API void TakeScreenshot(HWND in_window, HBITMAP *out_bitmap);

BITMAPS_API void ExtractSubImage(HBITMAP source_image, HBITMAP *destination_image,
  CRect subimage_position);
