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
// Purpose: mouse-handling for OpewnHoldems autoplayer.
//
//******************************************************************************

#include <afxwin.h>

enum MouseButton { MouseLeft, MouseMiddle, MouseRight };

int MouseClick(const HWND hwnd, const RECT rect, const MouseButton button, const int clicks);
int MouseClickDrag(const HWND hwnd, const RECT rect);
void ProcessMessage(const char *message, const void *param);

const POINT RandomizeClickLocation(const RECT rect);
const void GetClickPoint(const int x, const int y, const int rx, const int ry, POINT *p);