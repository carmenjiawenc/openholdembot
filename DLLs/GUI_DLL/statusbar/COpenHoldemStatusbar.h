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

#include <afxext.h>
#include <afxwin.h>

class COpenHoldemStatusbar {
 public:
	COpenHoldemStatusbar(CWnd *main_window);
	~COpenHoldemStatusbar();
 public:
	void OnUpdateStatusbar();
	void GetWindowRect(RECT *statusbar_position);
 private:
	void InitStatusbar();
  void UpdateValues();
  CString LastAction();
 private:
	CStatusBar _status_bar;
	CWnd *_main_window;
 private:
  int _handrank;
  double _prwin;
  double _prtie;
  double _prlos;
 private:
  // Info to be displayed
  CString _last_action;
  // to do!!! last line
  CString	_status_handrank;
  CString	_status_prwin;
};
