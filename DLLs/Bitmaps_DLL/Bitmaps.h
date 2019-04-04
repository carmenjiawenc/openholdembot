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

BITMAPS_API HBITMAP NewBitmap(int size_x, int size_y);
BITMAPS_API void DeleteBitmap(HBITMAP bitmap);

// Taking screenshots into pre-allocated bitmaps.
// The image is assumed to be as large as the screen / window.
// If the bitmap is smaller only a sub-image will be returned.
BITMAPS_API void ScreenToBitmap(HBITMAP out_bitmap);
BITMAPS_API void WindowToBitmap(HWND in_window, HBITMAP out_bitmap);
BITMAPS_API void WindowRegionToBitmap(HWND in_window, int pos_x, int pos_y, HBITMAP out_bitmap);

BITMAPS_API bool BitmapsAreEqual(HBITMAP HBitmapLeft, HBITMAP HBitmapRight) ;
BITMAPS_API void SaveBitmapToFile(HBITMAP bitmap, char* path);