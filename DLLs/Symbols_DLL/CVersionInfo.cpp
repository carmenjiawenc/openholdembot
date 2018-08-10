//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Provides version info for the debug-log
//   (version and filesize) to distinguish various versions of OH
//   and self-compiled derivates.
//   We have seen way too much "bug"-reports for older versions or modified code,
//   sometimes wasting a couple of nights with unneccessary debug-sessions.
//   Enough!
//
//******************************************************************************

#include "CVersionInfo.h"
#include <assert.h>
#include "..\Checksums_DLL\MD5_Checksum.h"
#include "..\Files_DLL\Files.h"
#include "..\Formula_DLL\CFormula.h"
#include "..\Formula_DLL\CFunctionCollection.h"
#include "..\GUI_DLL\resource.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\..\OpenHoldem\stdafx.h"

CVersionInfo::CVersionInfo()
{
	// Getting the check-sum only once on start-up
	_openholdem_MD5 = CalculateMD5(PathOfExecutable());
}

CVersionInfo::~CVersionInfo()
{}

CString CVersionInfo::GetVersionInfo()
{
	CString version_info;
  version_info.Format("OpenHoldem\n"
    "  Version  [%s, %s]\n"
    "  Formula  [%s]\n"
    "  Tablemap [%s]\n",
    VERSION_TEXT,
    _openholdem_MD5,
    Formula()->FunctionCollection()->FormulaName(),
    BasicScraper()->Tablemap()->filename());
	return version_info;
}