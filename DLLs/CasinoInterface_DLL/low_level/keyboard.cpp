//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: keyboard-handling for OpewnHoldems autoplayer.
//
//******************************************************************************

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#include <math.h>
#include "keyboard.h"
#include "mouse.h"
#include "..\..\Numerical_Functions_DLL\Numerical_Functions.h"

const int Shift   = 1;
const int Control = 2;
const int Alt     = 4;

void PlayKeyboardEvent(int vkey, int bscan)
{
	// This should hopefully fix the stuck control key problem.
	keybd_event(VK_CONTROL, 0, (bscan & Control) ? 0 : KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT,   0, (bscan & Shift)   ? 0 : KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU,    0, (bscan & Alt)     ? 0 : KEYEVENTF_KEYUP, 0);

	keybd_event(vkey,  bscan,  0, 0);
	keybd_event(vkey,  bscan,  KEYEVENTF_KEYUP, 0);

	if (bscan & Control)
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	if (bscan & Shift)
		keybd_event(VK_SHIFT,   0, KEYEVENTF_KEYUP, 0);
	if (bscan & Alt)
		keybd_event(VK_MENU,    0, KEYEVENTF_KEYUP, 0);
}

int SendString(const HWND hwnd, const RECT rect, const CString s, const bool use_comma)
{
	// Send each character of the string via PlayKeyboardEvent
	char ch_str[100];
	sprintf_s(ch_str, 100, "%s", s.GetString());

	int	vkey = 0;

	int i = 0, strlength = (int)strlen(ch_str);
	short KeyScan;
	for (int i=0; i<strlength; i++)
	{
		Sleep(20);
		if (use_comma && ch_str[i]=='.')
			ch_str[i] = ',';
		KeyScan = VkKeyScan(ch_str[i]);
		PlayKeyboardEvent(LOBYTE(KeyScan), HIBYTE(KeyScan));
	}
	Sleep(20);
	return (int) true;
}

int SendKey(const HWND hwnd, const RECT rect, UINT vkey)
{
	INPUT			input[4];

	POINT pt = RandomizeClickLocation(rect);
	double fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;

	// Translate click point to screen/mouse coords
	ClientToScreen(hwnd, &pt);
	double fx = pt.x*(65535.0f/fScreenWidth);
	double fy = pt.y*(65535.0f/fScreenHeight);

	// Set up the input structure
	int input_count=0;

	// First click in the rect to select it, if rect is not passed in as {-1, -1, -1, -1}
	if (rect.left!=-1 || rect.top!=-1 || rect.right!=-1 || rect.bottom!=-1)
	{
		ZeroMemory(&input[input_count],sizeof(INPUT));
		input[input_count].type = INPUT_MOUSE;
		input[input_count].mi.dx = (LONG) fx;
		input[input_count].mi.dy = (LONG) fy;
		input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
		input_count++;
		
		ZeroMemory(&input[input_count],sizeof(INPUT));
		input[input_count].type = INPUT_MOUSE;
		input[input_count].mi.dx = (LONG) fx;
		input[input_count].mi.dy = (LONG) fy;
		input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
		input_count++;
	}

	// Add vkey to the input struct
	ZeroMemory(&input[input_count],sizeof(INPUT));
	input[input_count].type = INPUT_KEYBOARD;
	input[input_count].ki.wVk = vkey;
	input_count++;

	ZeroMemory(&input[input_count],sizeof(INPUT));
	input[input_count].type = INPUT_KEYBOARD;
	input[input_count].ki.wVk = vkey;
	input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
	input_count++;

	// Set focus to target window
	SetFocus(hwnd);
	SetForegroundWindow(hwnd);
	SetActiveWindow(hwnd);

	// Send input
	SendInput(input_count, input, sizeof(INPUT));
  Sleep(20);
	return (int) true;
}