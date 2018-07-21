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
// Purpose: using the slider for allin-actions 
//   (2nd option after RaiseMax-button)
//
//******************************************************************************

#include <afxwin.h>
#include "LibDef.h"

class CASINO_INTERFACE_DLL_API CAllinSlider {
 public:
  CAllinSlider();
  ~CAllinSlider();
 public:
  void SetHandlePosition(const POINT position);
  void ResetHandlePosition();
  bool SlideAllin();
  bool GetSliderRegions();
  bool SlideAllinPossible();
 private:
  POINT _position; //?????
  RECT _i3_slider;
  RECT _i3_handle;
};
