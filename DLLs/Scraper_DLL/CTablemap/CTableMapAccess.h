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
// Purpose:
//
//*******************************************************************************

#include "..\LibDef.h"
#include "CTablemap.h"

enum dim {width, height};

class SCRAPER_DLL_API CTablemapAccess {
 public:
	CTablemapAccess();
	~CTablemapAccess();
 public:
	// r$
	bool GetButtonRect(CString button_name, RECT *_rect);
	bool GetTableMapRect(CString rect_name, RECT *_rect);
	bool GetTableMapRegion(CString region_name, STablemapRegion *_region);
 public:
	// s$
	bool SetTitleText(CString title_name, CString &destination);
 public:
	// z$
	// Usable for clientsizemin, clientsizemax and targetsize
  bool GetClientSize(CString size_name, int *width, int *height);
	unsigned int GetClientSize(CString size_name, dim dimension);
};