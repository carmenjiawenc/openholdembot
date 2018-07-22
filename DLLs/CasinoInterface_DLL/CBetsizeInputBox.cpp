//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: interface to the casinos input-box for betsizing
//
//******************************************************************************

#include "CBetsizeInputBox.h"
///#include "CAutoConnector.h"
///#include "CAutoplayerTrace.h"
#include "CCasinoInterface.h"
///#include "SwagAdjustment.h"
#include "low_level\keyboard.h"
#include "low_level\mouse.h"
#include "..\Debug_DLL\debug.h"
///#include "..\Formula_DLL\CAutoplayerLogic.h"
///#include "..\Formula_DLL\CFormula.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\StringFunctions_DLL\string_functions.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineHistory.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\WindowFunctions_DLL\window_functions.h"

CBetsizeInputBox::CBetsizeInputBox() {
  // dummy point for mouse and keyboard DLL
  p_null.x = kUndefined;
  p_null.y = kUndefined;
}

CBetsizeInputBox::~CBetsizeInputBox() {
}

bool CBetsizeInputBox::EnterBetsize(double total_betsize_in_dollars) {
  POINT	  cur_pos = { 0 };
  bool		lost_focus = false;
  // Not really (0, 0), but (-1, -1), out of the screen
  POINT	point_null = { kUndefined, kUndefined };
  CString	swag_amt;

  HWND foreground_window = GetForegroundWindow();
  CString foreground_title(" ", MAX_WINDOW_TITLE);
  WinGetTitle(foreground_window, foreground_title.GetBuffer());
  write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Foreground window: \"%s\"\n", foreground_title);
 write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Starting DoBetsize...\n");
  if (!GetI3EditRegion()) {
    write_log(k_always_log_errors, "[CBetsizeInputBox] WARNING! i3edit undefined or out of range\n");
    return false;
  }
  // In some cases only call and fold are possible.
  // Then a betsize should be skipped.
  // We detect this situation by missing min-raise button.
  // No backup-action here:
  // OH-script doesn't provide that and OPPL eill do that automatically.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=117&t=18125
  if (!CasinoInterface()->LogicalAutoplayerButton(k_autoplayer_function_raise)->IsClickable()) {
    write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] ...ending DoBetsize early (no (min-)raise possible).\n");
    return false;
  }
  // swag regions are hard coded as #3 for now, due to legacy WH standard
  if (!IsReadyToBeUsed()) { 
   write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] ...ending DoBetsize early (no edit field or no i3button).\n");
    return false;
  }
  SelectText();
  // First sleep(), THEN check for stolen focus, then act
  //  NOT the other way: http://www.maxinmontreal.com/forums/viewtopic.php?f=120&t=14791
  write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Sleeping %dms.\n", Preferences()->swag_delay_1());
  Sleep(Preferences()->swag_delay_1());
  // Check for stolen , and thus misswag
  if (CasinoInterface()->TableLostFocus()) {
    lost_focus = true;
  }
  Clear();
  write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Sleeping %dms.\n", Preferences()->swag_delay_2());
  Sleep(Preferences()->swag_delay_2());
  // Check for stolen focus, and thus misswag
  if (CasinoInterface()->TableLostFocus()) {
    lost_focus = true;
  }
  // SWAG AMOUNT ENTRY
  double swag_adjusted = 42;///!!!AdjustedBetsize(total_betsize_in_dollars);
  swag_amt = Number2CString(swag_adjusted);
  // Also adapt f$betsize for correct logging later-on
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=18648
  ///cyclic Formula()-> ()->SetAutoplayerFunctionValue(k_autoplayer_function_betsize, swag_adjusted);
  write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] betsize (not adjusted): %.2f\n", total_betsize_in_dollars);
  write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] calling keyboard.dll to enter betsize (adjusted): %s %d,%d %d,%d\n",
    swag_amt, _i3_edit_region.left, _i3_edit_region.top, _i3_edit_region.right, _i3_edit_region.bottom);
  bool use_comma_instead_of_dot = BasicScraper()->Tablemap()->use_comma_instead_of_dot();
  SendString(TableManagement()->AutoConnector()->attached_hwnd(), _i3_edit_region,
    swag_amt, use_comma_instead_of_dot);
  write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Sleeping %dms.\n", Preferences()->swag_delay_3());
  Sleep(Preferences()->swag_delay_3());
  // Check for stolen focus, and thus misswag
  if (CasinoInterface()->TableLostFocus()) {
    lost_focus = true;
  }
  // BET CONFIRMATION ACTION
  if (lost_focus) {
    // Print a very verbose warning in the log
    // Experience tell that beginenrs (and veterans) need that.
    write_log(k_always_log_errors, "[CBetsizeInputBox] WARNING! Betsizing failed because of lost focus.\n");
    write_log(k_always_log_errors, "[CBetsizeInputBox] Another window popped up and receives mouse and keyboard input.\n");
     write_log(k_always_log_errors, "[CBetsizeInputBox] This might be caused by bad casino, bad hopper or by user-interaction.\n");
  } else {
    if (BasicScraper()->Tablemap()->swagconfirmationmethod() == BETCONF_ENTER) {
      write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Confirmation; calling keyboard.dll to press 'Enter'\n");
      CasinoInterface()->SendKey(VK_RETURN);
    } else if (BasicScraper()->Tablemap()->swagconfirmationmethod() == BETCONF_CLICKBET
      && CasinoInterface()->LogicalAutoplayerButton(k_autoplayer_function_raise)->IsClickable()) {
      write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Bet Confirmation: Using raise button\n");
      if (BasicScraper()->Tablemap()->buttonclickmethod() == BUTTON_DOUBLECLICK) {
        CasinoInterface()->ClickButtonSequence(k_autoplayer_function_raise,
          k_autoplayer_function_raise,
          k_double_click_delay);
      } else {
        CasinoInterface()->LogicalAutoplayerButton(k_autoplayer_function_raise)->Click();
      }
    } else if (BasicScraper()->Tablemap()->swagconfirmationmethod() == BETCONF_NOTHING) {
    } else {
      write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] ...ending DoBetsize early (invalid betsizeconfirmationmethod).\n");
      write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Valid options are: \"enter\", \"click bet\", \"nothing\"\n");
      return false;
    }
  }
  /*#int betround = BetroundCalculator()->betround();
  write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] ...ending DoBetsize, 'didbetsize' now: %d\n",
    EngineContainer()->symbol_engine_history()->didswag(betround));*/
  return (!lost_focus); 
}

bool CBetsizeInputBox::GetI3EditRegion() {
  BasicScraper()->Tablemap()->GetTMRegion("i3edit", &_i3_edit_region);
  if ((_i3_edit_region.bottom < 0)
      || (_i3_edit_region.left < 0)
      || (_i3_edit_region.right < 0)
      || (_i3_edit_region.top < 0)) {
    return false;
  }
  return true;
}

bool CBetsizeInputBox::IsReadyToBeUsed() {
  if (BasicScraper()->Tablemap()->swagconfirmationmethod() == BETCONF_CLICKBET) {
    if (!CasinoInterface()->BetsizeConfirmationButton()->IsClickable()) {
      return false;
    }
  }
  if (!BasicScraper()->Tablemap()->ItemExists("i3edit")) {
    return false;
  }
  if (!GetI3EditRegion()) {
    return false;
  }
  return true;
}

void CBetsizeInputBox::SelectText() {
  if (BasicScraper()->Tablemap()->swagselectionmethod() == TEXTSEL_SINGLECLICK) {
   write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Text selection; calling mouse.dll to single click: %d,%d %d,%d\n",
      _i3_edit_region.left, _i3_edit_region.top, _i3_edit_region.right, _i3_edit_region.bottom);
    MouseClick(TableManagement()->AutoConnector()->attached_hwnd(), _i3_edit_region, MouseLeft, 1);
  } else if (BasicScraper()->Tablemap()->swagselectionmethod() == TEXTSEL_DOUBLECLICK) {
   write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Text selection; calling mouse.dll to double click: %d,%d %d,%d\n",
      _i3_edit_region.left, _i3_edit_region.top, _i3_edit_region.right, _i3_edit_region.bottom);
    MouseClick(TableManagement()->AutoConnector()->attached_hwnd(), _i3_edit_region, MouseLeft, 2);
  } else if (BasicScraper()->Tablemap()->swagselectionmethod() == TEXTSEL_TRIPLECLICK) {
   write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Text selection; calling mouse.dll to triple click: %d,%d %d,%d\n",
      _i3_edit_region.left, _i3_edit_region.top, _i3_edit_region.right, _i3_edit_region.bottom);
    MouseClick(TableManagement()->AutoConnector()->attached_hwnd(), _i3_edit_region, MouseLeft, 3);
  } else if (BasicScraper()->Tablemap()->swagselectionmethod() == TEXTSEL_CLICKDRAG) {
   write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Text selection; calling mouse.dll to click drag: %d,%d %d,%d\n",
      _i3_edit_region.left, _i3_edit_region.top, _i3_edit_region.right, _i3_edit_region.bottom);
    MouseClickDrag (TableManagement()->AutoConnector()->attached_hwnd(), _i3_edit_region);
  } else if (BasicScraper()->Tablemap()->swagselectionmethod() == TEXTSEL_NOTHING) {
    // Nothing to do
  } else {
   write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] ...ending DoBetsize early (invalid betsizeselectionmethod).\n");
    return;
  }
}

void CBetsizeInputBox::Clear() {
  if (BasicScraper()->Tablemap()->swagdeletionmethod() == TEXTDEL_DELETE) {
    write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Text deletion; calling keyboard.dll to press 'delete'\n");
    CasinoInterface()->SendKey(VK_DELETE);
  }
  else if (BasicScraper()->Tablemap()->swagdeletionmethod() == TEXTDEL_BACKSPACE) {
    write_log(Preferences()->debug_autoplayer(), "[CBetsizeInputBox] Text deletion; calling keyboard.dll to press 'backspace'\n");
     CasinoInterface()->SendKey(VK_BACK);
  }
  else if (BasicScraper()->Tablemap()->swagdeletionmethod() == TEXTDEL_NOTHING) {
    // Nothing to do to delete the text
    // Once selected it will be overwritten.
  }
}

void CBetsizeInputBox::Confirm() {
}

bool CBetsizeInputBox::VerifyEnteredBetsize() {
  // For future use
  return true;
}
