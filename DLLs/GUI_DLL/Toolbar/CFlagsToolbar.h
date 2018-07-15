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
 public: /// To be refactored to get rid of dependencies!!!
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


/*#
void CAutoplayer::EngageAutoplayer(bool to_be_enabled_or_not) {
ENT
// Set correct button state
// We have to be careful, as during initialization the GUI does not yet exist.
assert(p_flags_toolbar != NULL);
GUI()->FlagsToolbar()->CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, to_be_enabled_or_not);

if (to_be_enabled_or_not)
{
if (!Formula()->FunctionCollection()->BotLogicCorrectlyParsed())
{
// Invalid formula
// Can't autoplay
to_be_enabled_or_not = false;
}
}
if (to_be_enabled_or_not) {
GUI()->FlagsToolbar()->ResetButtonsOnAutoplayerOn();
} else {
GUI()->FlagsToolbar()->ResetButtonsOnAutoplayerOff();
}
// Set value at the very last to be extra safe
// and avoid problems with multiple threads
// despite we use synchronization ;-)
_autoplayer_engaged = to_be_enabled_or_not;
}

*/