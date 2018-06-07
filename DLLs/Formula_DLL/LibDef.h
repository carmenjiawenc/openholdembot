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
// Purpose: single import/export macro for all header-files
//
//******************************************************************************

#ifdef FORMULA_DLL_EXPORTS
#define FORMULA_DLL_API extern "C" __declspec(dllexport)
#else
#define FORMULA_DLL_API extern "C" __declspec(dllimport)
#endif