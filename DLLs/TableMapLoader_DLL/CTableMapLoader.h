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
//   * loading all tablemaps and extract connection-data for the auto-connector
//   * checking if a map matches
//
//******************************************************************************

#include "CFileSystemMonitor.h"
#include "LibDef.h"
#include "..\MemoryManagement_DLL\CSpaceOptimizedGlobalObject.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"

// This function has to be global and can't be part of the class,
// as it has to be called by the callback-function 
// BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
TABLEMAP_LOADER_DLL_API bool Check_TM_Against_Single_Window(int MapIndex, HWND h);

class TABLEMAP_LOADER_DLL_API CTableMapLoader //!!! public CSpaceOptimizedGlobalObject 
{
 public:
	CTableMapLoader();
	~CTableMapLoader();
 public:
	int NumberOfTableMapsLoaded() { return _number_of_tablemaps_loaded; }
	void ReloadAllTablemapsIfChanged();
	CString GetTablemapPathToLoad(int tablemap_index);
 private:
	void ParseAllTableMapsToLoadConnectionData(CString scraper_directory);
	void ParseAllTableMapsToLoadConnectionData();
	bool tablemap_connection_dataAlreadyStored(CString TablemapFilePath);
	void CheckForDuplicatedTablemaps();
	void ExtractConnectionDataFromCurrentTablemap(CTablemap *cmap);
 private:
	bool	tablemaps_in_scraper_folder_already_parsed;
	int	_number_of_tablemaps_loaded;
 private:
  CFileSystemMonitor filesystem_monitor;
};

typedef struct {
  CString	FilePath;
  CString	SiteName;
  int	    ClientSizeMinX, ClientSizeMinY;
  int	    ClientSizeMaxX, ClientSizeMaxY;
  CString	TitleText;
  CString	TitleText_0_9[10];
  CString	NegativeTitleText;
  CString	NegativeTitleText_0_9[10];
  STablemapRegion	TablePoint[10];
  int			TablePointCount;
} t_tablemap_connection_data;

extern std::map<int, t_tablemap_connection_data> tablemap_connection_data;

TABLEMAP_LOADER_DLL_API CTableMapLoader* TableMapLoader();