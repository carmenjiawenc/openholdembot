#include-once
#include <WinAPI.au3>


Global Const $gui_title = "OpenHoldem EasyScraper"
Global Const $GUI_width = 330
Global $preview_temp_filename = "OpenHoldem_EasyScraper.bmp"

#include <Array.au3>
#include <MainGUI.au3>

; Do niot access this variable directly
; Use the accessor ConnectedWindow(). Ot catches disappearing windows
Global $connected_window
; inner size of the connected window
; target-size of the tablemap
Global $targetsize[2] = [640, 480]

; x, y, colour  (2nd last mouse position)
; x, y, colour  (last mouse position)
; x, y, colour  (current mouse position)
Global $current_region_data[9] = [0, 0, 0, 0, 0, 0, 0, 0, 0]
Global $mouseclicks_collected = 0

Global Enum $task_none = -1, $task_seated, $task_active, $task_dealer, $task_nocard, _
	$task_cardbacks, $task_balances, $task_bets, $task_names, $task_button_fold, _
	$task_button_check, $task_button_call, $task_button_minraise, $task_button_allin
Global $current_task = $task_none
; $coords_required_per_task does not include $task_none
; This gets handled extra
Global $coords_required_per_task [13] = [1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3]
Global $partial_region_name_for_task[13] = ["seated", "active", "dealer", "nocard","cardback", _
	"balance", "bet", "name", "button", "button", "button", "button", "button"]
; Only some asks need a font-file, but it is more convenient this way
Global $font_file_for_task[13]


Const $buttons_to_be_collected = 5
Const $default_colour_radius = 5

; Countin players and buttons
Global $index_regions_for_current_task

; two arrays for names and values of collected regions
Global $region_collection_keys[1]
Global $region_collection_values[1]



Global $symbols_data[13][2]


Func ConnectedWindow()
	If WinExists($connected_window) Then
		Return $connected_window
	Else
		Return _WinAPI_GetDesktopWindow()
	EndIf
EndFunc

Func RegionCollection_Add($key, $value)
	Local $index = RegionCollection_GetIndex($key)
	If $index >= 0 Then
		$region_collection_keys[$index]= $key
		$region_collection_values[$index] = $value
	Else
		_ArrayAdd($region_collection_keys, $key)
		_ArrayAdd($region_collection_values, $value)
	Endif
	AppendLineToEditLog(RegionCollection_FormatLine($key, $value))
EndFunc

Func RegionCollection_GetIndex($key)
	Local $index = _ArraySearch($region_collection_keys, $key)
	If @error Then
		Return -1
	EndIf
	Return $index
EndFunc

Func RegionCollection_FormatLine($key, $value)
	Return StringFormat("%-20s  %s", $key, $value)
EndFunc

Func RegionCollection_GetAll()
	Local $result
	; Skip index 0, which is always empty
	For $i = 1 To (UBound($region_collection_keys) - 1)
		Local $nextline = RegionCollection_FormatLine($region_collection_keys[$i], $region_collection_values[$i]) & @CRLF
		$Result &= $nextline
	Next
	Return $result
EndFunc