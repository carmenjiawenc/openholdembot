//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: using the slider for allin-actions 
//   (2nd option after RaiseMax-button)
//
//******************************************************************************

#include "CAllinSlider.h"
//#include "CAutoConnector.h"
#include "CBetsizeInputBox.h"
#include "CCasinoInterface.h"
#include "low_level\mouse.h"
#include "..\Debug_DLL\debug.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Scraper_DLL\CBasicScraper.h"
#include "..\Scraper_DLL\CTablemap\CTablemap.h"
#include "..\TableManagement_DLL\CTableManagement.h"
#include "..\..\OpenHoldem\OpenHoldem.h"

CAllinSlider::CAllinSlider() {
  ResetHandlePosition();
  // Not really (0, 0), but (-1, -1), out of the screen
  POINT	point_null = { kUndefined, kUndefined };
}

CAllinSlider::~CAllinSlider() {
}

void CAllinSlider::SetHandlePosition(const POINT position) {
  _position.x = position.x;
  _position.y = position.y;
}

void CAllinSlider::ResetHandlePosition() {
  _position.x = kUndefined;
  _position.y = kUndefined;
}

bool CAllinSlider::SlideAllin() {
  if (!SlideAllinPossible()) {
   write_log(Preferences()->debug_autoplayer(), "[AllinSlider] ...ending DoSlider early (i3handle or i3slider are not defined in the tablemap)\n");
    return false;
  }
 write_log(Preferences()->debug_autoplayer(), "[AllinSlider] Starting DoSlider...\n");
  if ((_position.x == kUndefined) || (_position.y == kUndefined)) {
   write_log(Preferences()->debug_autoplayer(), "[AllinSlider] ...ending DoSlider early (handle not found - i3handle must use a transform that resolves to either 'handle' or 'true')\n");
    return false;
  }
  // Click and drag handle
  RECT drag_region;
  GetSliderRegions();
  drag_region.left = _position.x + ((_i3_handle.right - _i3_handle.left) / 2);
  drag_region.top = _position.y + ((_i3_handle.bottom - _i3_handle.top) / 2);
  drag_region.right = _position.x + (_i3_slider.right - _i3_slider.left);
  drag_region.bottom = drag_region.top;

  write_log(Preferences()->debug_autoplayer(), "[AllinSlider] Slider : Calling mouse.dll to jam from %d,%d to %d,%d\n", drag_region.left, drag_region.top, drag_region.right, drag_region.bottom);
  // Not really (0, 0), but (-1, -1), out of the screen
  POINT	point_null = { kUndefined, kUndefined };
  MouseClickDrag (TableManagement()->AutoConnector()->attached_hwnd(), drag_region);

  write_log(Preferences()->debug_autoplayer(), "[AllinSlider] Sleeping %d ms\n.", Preferences()->swag_delay_3());
  Sleep(Preferences()->swag_delay_3());

  // Click confirmation button 
  CasinoInterface()->_betsize_input_box.Confirm();
  write_log(Preferences()->debug_autoplayer(), "[AllinSlider] Jam complete: %d,%d,%d,%d\n", drag_region.left, drag_region.top, drag_region.right, drag_region.bottom);
  write_log(Preferences()->debug_autoplayer(), "[AllinSlider] ...ending DoSlider.\n");
  return true;
}

bool CAllinSlider::GetSliderRegions() {
  BasicScraper()->Tablemap()->GetTMRegion("i3slider", &_i3_slider);
  BasicScraper()->Tablemap()->GetTMRegion("i3handle", &_i3_handle);
  if ((_i3_slider.bottom < 0)
    || (_i3_slider.left < 0)
    || (_i3_slider.right < 0)
    || (_i3_slider.top < 0)) {
    return false;
  }
  if ((_i3_handle.bottom < 0)
    || (_i3_handle.left < 0)
    || (_i3_handle.right < 0)
    || (_i3_handle.top < 0)) {
    return false;
  }
  return true;
}

bool CAllinSlider::SlideAllinPossible() {
  // Required: betsize-confirmation-button, slider and handle
  if (BasicScraper()->Tablemap()->swagconfirmationmethod() == BETCONF_CLICKBET) {
    if (!CasinoInterface()->BetsizeConfirmationButton()->IsClickable()) {
      return false;
    }
  }
  if (!BasicScraper()->Tablemap()->ItemExists("i3slider")) {
    return false;
  }
  if (!BasicScraper()->Tablemap()->ItemExists("i3handle")) {
    return false;
  }
  if (!GetSliderRegions()) {
    return false;
  }
  return true;
}