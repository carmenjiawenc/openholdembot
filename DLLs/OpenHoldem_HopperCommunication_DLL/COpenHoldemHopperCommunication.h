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

#include <afxwin.h>

class COpenHoldemHopperCommunication: public CWnd {
 public:
	COpenHoldemHopperCommunication();
	~COpenHoldemHopperCommunication();
 public:
	LRESULT OnConnectedHwndMessage(WPARAM, LPARAM);
	LRESULT OnSetFlagMessage(WPARAM, LPARAM flag_to_set);
	LRESULT OnResetFlagMessage(WPARAM, LPARAM flag_to_reset);
 protected:
	///DECLARE_MESSAGE_MAP()
};

/*#
todo: docu removed functionality
LRESULT OnIsReadyMessage(WPARAM, LPARAM);
LRESULT OnSetWindowText(WPARAM, LPARAM title);
LRESULT OnConnectMessage(WPARAM, LPARAM hwnd);
LRESULT OnDisconnectMessage(WPARAM, LPARAM);
*/