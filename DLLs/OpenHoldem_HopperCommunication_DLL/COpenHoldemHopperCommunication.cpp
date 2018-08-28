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

#include <afxwin.h>
#include "..\Debug_DLL\debug.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineFlags.h"
#include "..\TableManagement_DLL\CAutoConnector.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

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

///IMPLEMENT_DYNAMIC(COpenHoldemHopperCommunication, CWnd)

BEGIN_MESSAGE_MAP(COpenHoldemHopperCommunication, CWnd)  
END_MESSAGE_MAP()

LRESULT COpenHoldemHopperCommunication::OnConnectedHwndMessage(WPARAM, LPARAM)
{
	write_log(Preferences()->debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8004: OnConnectedHwndMessage\n");
	return (LRESULT)TableManagement()->AutoConnector()->attached_hwnd();
}


LRESULT COpenHoldemHopperCommunication::OnSetFlagMessage(WPARAM, LPARAM flag_to_set)
{
	write_log(Preferences()->debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8005: OnSetFlagMessage\n");
	if ((flag_to_set < 0) || (flag_to_set >= kNumberOfFlags))
	{
		return false;
	}
  EngineContainer()->symbol_engine_flags()->SetFlag(int(flag_to_set), true);
	return true;
}

LRESULT COpenHoldemHopperCommunication::OnResetFlagMessage(WPARAM, LPARAM flag_to_reset)
{
	write_log(Preferences()->debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8006: OnResetFlagMessage\n");
	if ((flag_to_reset < 0) || (flag_to_reset >= kNumberOfFlags))
	{
		return false;
	}
  EngineContainer()->symbol_engine_flags()->SetFlag(int(flag_to_reset), false);
	return true;
}