#Include <WinAPI.au3>

Const $k_mouse_speed_slowesr =  1
Const $k_mouse_speed_dastesr = 20

; Initializing to default
; Should be defined later with the real value
Global $mouse_initial_speed = 10

Func RemeberMouseSpeed()
	; https://www.autoitscript.com/forum/topic/134010-change-mousespeed-via-autoit-like-in-system-settings/
	$mouse_initial_speed = RegRead("HKCU\Control Panel\Mouse", "MouseSensitivity")
EndFunc

Func SetMouseSpeed($speed)
	; https://www.autoitscript.com/forum/topic/134010-change-mousespeed-via-autoit-like-in-system-settings/
	_WinAPI_SystemParametersInfo(113, 0, $speed, 2) ; 20 = fastest
EndFunc

Func ResetMouseSpeed()
	SetMouseSpeed($mouse_initial_speed)
EndFunc

Func SetSlowestMouseSpeed()
	SetMouseSpeed($k_mouse_speed_slowesr)
EndFunc
