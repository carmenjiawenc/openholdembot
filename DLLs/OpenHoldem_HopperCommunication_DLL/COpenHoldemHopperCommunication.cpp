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

#define OPENHOLDEM_HOPPER_COMMUNICATION_DLL_EXPORTS

#include "COpenHoldemHopperCommunication.h"
/*##include "CAutoConnector.h"
#include "CFlagsToolbar.h"
#include "CFormulaParser.h"
#include "CFunctionCollection.h"
#include "COpenHoldemTitle.h"

#include "CTableMaploader.h"
#include "MainFrm.h"
#include "OpenHoldem.h"*/

#include <afxwin.h>
#include "..\Debug_DLL\debug.h"
#include "..\GUI_DLL\CGUI.h"
#include "..\GUI_DLL\Toolbar\CFlagsToolbar.h"
#include "..\Preferences_DLL\Preferences.h"

// WM_APP = 0x8000 = 32768
// Messages 0..WM_APP reserved for Windows
// Messages (WM_APP + 1)..0xBFFF user-defined
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms644930(v=vs.85).aspx
// Some of them now unused, we keep them (especially the values)
// fpr cpmpatibility
const int WMA_SETWINDOWTEXT = WM_APP + 1;
const int WMA_DOCONNECT = WM_APP + 2;
const int WMA_DODISCONNECT = WM_APP + 3;
const int WMA_CONNECTEDHWND = WM_APP + 4;
const int WMA_SETFLAG = WM_APP + 5;
const int WMA_RESETFLAG = WM_APP + 6;
const int WMA_ISREADY = WM_APP + 7;

//IMPLEMENT_DYNAMIC(COpenHoldemHopperCommunication, CWnd)

///BEGIN_MESSAGE_MAP(COpenHoldemHopperCommunication, CWnd)  
///END_MESSAGE_MAP()

LRESULT COpenHoldemHopperCommunication::OnConnectedHwndMessage(WPARAM, LPARAM)
{
	write_log(Preferences()->debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8004: OnConnectedHwndMessage\n");
	return (LRESULT) 42;///p_autoconnector->attached_hwnd();
}


LRESULT COpenHoldemHopperCommunication::OnSetFlagMessage(WPARAM, LPARAM flag_to_set)
{
	write_log(Preferences()->debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8005: OnSetFlagMessage\n");
	int _flag_to_set = int(flag_to_set);
	if ((_flag_to_set < 0) || (_flag_to_set >= kNumberOfFlags))
	{
		return false;
	}
  ///!!! Verify that pointers are not NULL if we work without gUI
  GUI()->FlagsToolbar()->SetFlag(_flag_to_set, true);
	return true;
}

LRESULT COpenHoldemHopperCommunication::OnResetFlagMessage(WPARAM, LPARAM flag_to_reset)
{
	write_log(Preferences()->debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8006: OnResetFlagMessage\n");
	int _flag_to_reset = int(flag_to_reset);
	if ((_flag_to_reset < 0) || (_flag_to_reset >= kNumberOfFlags))
	{
		return false;
	}
  ///!!! Verify that pointers are not NULL if we work without gUI
  GUI()->FlagsToolbar()->SetFlag(_flag_to_reset, false);
	return true;
}