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
// Purpose: interface to the flags toolbar with buttons 0..19
//   The flags-toolbar provides an easy method to configure the
//   botlogic via the OpenHoldem GUI during development..
//
//******************************************************************************

#include <afxext.h>
#include "MyCToolBar.h"
#include "..\..\..\Shared\CCritSec\CCritSec.h"
#include "..\..\..\Shared\MagicNumbers\MagicNumbers.h"

class CFlagsToolbar: public CWnd {
 public:
	CFlagsToolbar(CFrameWnd *parent_window);
	~CFlagsToolbar();
 public:
  void SetFlag(int flag_number, bool new_value);
 public:
	void ResetButtonsOnConnect();
	void ResetButtonsOnDisconnect();
  void ResetButtonsOnAutoplayerOn();
  void ResetButtonsOnAutoplayerOff();
	void UnattachOHFromPokerWindow();
 public:
	void EnableButton(int button_ID, bool new_status);
	void CheckButton(int button_ID, bool new_status);
	bool IsButtonChecked(int button_ID);
	bool IsButtonEnabled(int button_ID);
 public:
 public:
	afx_msg void OnClickedFlags();
	DECLARE_MESSAGE_MAP()
 public:
  CMyToolBar _tool_bar;
 private:
	void CreateMainToolbar();
	void CreateFlagsToolbar();
	void AlignToolbars();
 private:
  CMyToolBar  m_MainToolBar;
	bool       _flags[kNumberOfFlags];
	CFrameWnd  *_parent_window;
	CCritSec   m_critsec;
};
