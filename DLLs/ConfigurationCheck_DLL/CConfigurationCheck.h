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
//
//******************************************************************************

#include "LibDef.h"
#include <afxwin.h>
#include "..\MemoryManagement_DLL\CSpaceOptimizedGlobalObject.h"

class CONFIGURATION_CHECK_DLL_API CConfigurationCheck: public CSpaceOptimizedGlobalObject {
 public:
	CConfigurationCheck();
	~CConfigurationCheck();
 public:
  // For Menu -> ProblemSolver
  void ForceAllConfigurationChercks();
 private:
  void CheckEnabledConfigurationChecks(bool force_all);
 private:
	HKEY GetHive(CString mhive);
	bool OpenKey(CString mhive, CString registry_path);
	CString GetValue(CString mhive, int type, CString registry_path, CString key_name);
	void CheckColourDepth();
	void CheckInputSettings();
	void CheckForSwapMouseBtns();
	void CheckForFontSmoothing();
};