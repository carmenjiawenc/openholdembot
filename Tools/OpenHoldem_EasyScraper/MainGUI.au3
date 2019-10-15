#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Res_Icon_Add=OpenHoldem_EasyScraper.ico
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

#include-once

#include <ClientAreaOffset.au3>
#include <Data.au3>
#include <GuiEdit.au3>
#include <MsgBoxUDF.au3>
#include <Process.au3>
#include <ScreenCapture.au3>
#include <ScrollBarConstants.au3>
#include <WinAPI.au3>

Const $preview_original_width  = 30
Const $preview_original_geight = 12
Const $preview_zoom_factor     =  8

Func PositionedMessageBox($i_flag, $s_title, $s_text)
	; Requested by Poker_dog for very large screens with scrollbars
	; http://www.maxinmontreal.com/forums/viewtopic.php?f=261&t=22521#p158277
	_MsgBox_SetWindowPos(200, 200)
	_MsgBox($i_flag, $s_title, $s_text)
EndFunc

Func ShowLicense()
	PositionedMessageBox(0, "About", _
		$gui_title & @CRLF & _
        "Version 13.1.2" & @CRLF & _
        "Licensed under GPLv3" & @CRLF & _
		"" & @CRLF & _
        "http://www.maxinmontreal.com/forums/index.php" & @CRLF & _
        "https://github.com/OpenHoldem/openholdembot/" & @CRLF & _
        "http://www.gnu.org/licenses/gpl.html" & @CRLF & _
		"" & @CRLF & _
        "Disclaimer: this is an open-source project." & @CRLF & _
        "We are not related to any shops, stores and/or scam-artists.")
EndFunc

Func CreateGUI()
    ; Create GUI
	Global $main_GUI = GUICreate($gui_title, $GUI_width, 610)
	Const $button_width = 150
	Const $input_width = 150
	Const $combo_width = 50
	Const $left_position_buttons = 10
	Const $left_position_labels = 60
	Const $left_position_editables = $left_position_buttons + $button_width + 10
	Local $y_position = 10
	Const $cell_height = 30
	Global $button_connect = GUICtrlCreateButton("Connect", $left_position_buttons, $y_position, $button_width)
	Global $button_about = GUICtrlCreateButton("About", $left_position_editables, $y_position, $button_width)
	Const $small_vertical_space = 20
	$y_position += $cell_height
	$y_position += $small_vertical_space
	GUICtrlCreateLabel("Title Identifier", $left_position_labels, $y_position)
	Global $edit_table_identifier = GUICtrlCreateInput("", $left_position_editables, $y_position, $input_width)
	$y_position += $cell_height
	GUICtrlCreateLabel("Target Size", $left_position_labels, $y_position)
	Global $label_clientsize = GUICtrlCreateLabel("0  0", $left_position_editables, $y_position, $input_width)
	$y_position += $cell_height
	GUICtrlCreateLabel("Size Tolerance %", $left_position_labels, $y_position)
	Global $combo_size_tolerance = GUICtrlCreateCombo("40", $left_position_editables, $y_position, $combo_width)
    GUICtrlSetData(-1, "30|20|10|0", "40")
	$y_position += $cell_height
	;GUICtrlCreateLabel("Process", $left_position_labels, $y_position)
	;Global $edit_executable = GUICtrlCreateInput("", $left_position_editables, $y_position, $input_width)
	;$y_position += $cell_height
	GUICtrlCreateLabel("Chairs", $left_position_labels, $y_position, $combo_width)
	Global $combo_chairs = GUICtrlCreateCombo("10", $left_position_editables, $y_position)
    GUICtrlSetData(-1, "9|8|7|6|5|4|3|2", 10)
	$y_position += $cell_height
	$y_position += $small_vertical_space
	Global $button_seated = GUICtrlCreateButton("Seated", $left_position_buttons, $y_position, $button_width)
	Global $button_symbols = GUICtrlCreateButton("Symbols", $left_position_editables, $y_position, $button_width)
	$y_position += $cell_height
	Global $button_active = GUICtrlCreateButton("Sitting Out", $left_position_buttons, $y_position, $button_width)
	Global $button_balances = GUICtrlCreateButton("Balances", $left_position_editables, $y_position, $button_width)
	$y_position += $cell_height
	Global $button_dealer = GUICtrlCreateButton("Dealer", $left_position_buttons, $y_position, $button_width)
	Global $button_bets = GUICtrlCreateButton("Bets", $left_position_editables, $y_position, $button_width)
	$y_position += $cell_height
	Global $button_cardback = GUICtrlCreateButton("Cardbacks", $left_position_buttons, $y_position, $button_width)
	Global $button_pots = GUICtrlCreateButton("Pots", $left_position_editables, $y_position, $button_width)
	$y_position += $cell_height
	Global $button_nocard = GUICtrlCreateButton("NoCard", $left_position_buttons, $y_position, $button_width)
	Global $button_names = GUICtrlCreateButton("Names", $left_position_editables, $y_position, $button_width)
	$y_position += $cell_height
	Global $button_cardfaces = GUICtrlCreateButton("CardFaces", $left_position_buttons, $y_position, $button_width)
	Global $button_buttons = GUICtrlCreateButton("Buttons", $left_position_editables, $y_position, $button_width)
	$y_position += $cell_height
	Global $edit_log = GUICtrlCreateEdit("", $left_position_buttons, $y_position, 310, 90)
	_GUICtrlEdit_SetReadOnly($edit_log, True)
	$y_position += $cell_height
	$y_position += $cell_height
	$y_position += $cell_height
	$y_position += $small_vertical_space
	; Preview regions
	; Global $label_preview_text = GUICtrlCreateLabel("", $left_position_buttons, $y_position, 200, 56)
	; GUICtrlSetBkColor($label_preview_text, 0)
	Local $preview_width  = $preview_original_width  * $preview_zoom_factor
	Local $preview_height = $preview_original_geight * $preview_zoom_factor
	Global $preview_picture = GUICtrlCreatePic("", $left_position_buttons + 4, $y_position + 4, 192, 48)
	Global $label_preview_colour = GUICtrlCreateLabel("", 264, $y_position, 56, 56)
	GUICtrlSetBkColor($label_preview_colour, 0)
	$y_position += $cell_height
	$y_position += $cell_height
	$y_position += $small_vertical_space
	; Final save button
	Global $button_save = GUICtrlCreateButton("Save", $left_position_buttons, $y_position, $button_width)
	$y_position += $cell_height
	GUISetIcon("OpenHoldem_EasyScraper.ico")
    GUISetState()
EndFunc

Func InstructToCollectNoCards()
	PositionedMessageBox(0, "Next Task", "Please collect 'nocard'-regioms." & @CRLF & _
		"Two pixel per player, starting at 1 o'clock position clockwise." & @CRLF & _
		"Move the mouse, then press F5.")
EndFunc

Func InstructToCollectPixels($type)
	PositionedMessageBox(0, "Next Task", "Please collect pixels for '" & $type & "'-regioms." & @CRLF & _
		"One pixel per player, starting at 1 o'clock position clockwise." & @CRLF & _
		"Move the mouse, then press F5.")
EndFunc

Func InstructToCollectBetsBalancesNames()
	Local $region_type = $partial_region_name_for_task[$current_task] & "s"
	PositionedMessageBox(0, "Next Task", "Please collect pixels for '" & $region_type & "'." & @CRLF & _
		"Two pixels per player, starting at 1 o'clock position clockwise." & @CRLF & _
		"* one pixel in the middle of the region" & @CRLF & _
		"* one pixel for the text-colour")
EndFunc

Func InstructToCollectButton($type)
	PositionedMessageBox(0, "Next Task", "Please create autoplayer-button '" & $type & "'." & @CRLF & _
		"* one inner pixel near top-left" & @CRLF & _
		"* one inner pixel near bottom-right" & @CRLF & _
		"* one inner pixel for the colour" & @CRLF)
EndFunc

Func InstructToCollectNextButton()
	If $current_task = $task_button_check Then
		InstructToCollectButton("check")
	ElseIf $current_task = $task_button_call Then
		InstructToCollectButton("call")
	ElseIf $current_task = $task_button_minraise Then
		InstructToCollectButton("minraise")
	ElseIf $current_task = $task_button_allin Then
		InstructToCollectButton("allin")
	Else
		; Last button reached
		; $current_task points into nirvana after $task_button_allin
		$current_task = $task_button_allin
	EndIf
EndFunc   ;==>InstructToCollectNextButton

Func UpdateColourDisplay()
	Global $last_update_time
	If ($last_update_time = @SEC) Then
		Return
	EndIf
	$last_update_time = @SEC
	Local $position = MouseGetPos()
	Local $colour = PixelGetColor($position[0], $position[1])
	GUICtrlSetBkColor($label_preview_colour, $colour)
EndFunc

Func UpdatePreviewDisplay()
	Const $half_width = 1/2 * $preview_original_width
	Const $half_height = 1/2 * $preview_original_geight
	Local $position = MouseGetPos()
	Local $image = _ScreenCapture_Capture($preview_temp_filename, $position[0] - $half_width, $position[1] - $half_height,  $position[0] + $half_width, $position[1] + $half_height)
	GUICtrlSetImage($preview_picture, $preview_temp_filename)
EndFunc

Func AppendLineToEditLog($line)
	Local $data = GUICtrlRead($edit_log)
	$data &= $line
	$data &= @CRLF
	GUICtrlSetData($edit_log, $data)
	_GUICtrlEdit_Scroll($edit_log, $SB_SCROLLCARET)
EndFunc

Func ClearEditLog()
	GUICtrlSetData($edit_log, "")
EndFunc

Func PositionMyGUI()
	Local $position_table = WinGetPos(ConnectedWindow())
	Local $table_width = $position_table[2]
	Local $screen_width = @DeskTopWidth
	Local $main_gui_x_position = 640
	If ($table_width + $GUI_width > $screen_width) Then
		$main_gui_x_position = $screen_width - $GUI_width
	Else
		$main_gui_x_position = $table_width
	EndIf
	WinMove($gui_title, "", $main_gui_x_position, 0)
EndFunc

Func Connect()
	PositionedMessageBox(0, "Connect to PokerTable", _
		"Please close this popup, then activate the poker-table in order to select it.")
	Do
		Sleep(250)
		Do
			$connected_window = WinGetHandle("")
			Local $process_ID = WinGetProcess(ConnectedWindow())
			Local $process_name = _ProcessGetName($process_ID)
		Until ($process_name <> "AutoIT3.exe") AND ($process_name <> @ScriptName)
		Local $title = WinGetTitle(ConnectedWindow())
		GUICtrlSetData($edit_table_identifier, $title)
		$targetsize = WinGetClientSize(ConnectedWindow())
		; WinGetClientSize sometimes fails for unknown reasons
		; Working around it with the outer loop
	Until IsArray($targetsize)
	PositionMyGUI()
	ClearEditLog()
	Local $clientsize_text = $targetsize[0] & "  " & $targetsize[1]
	GUICtrlSetData($label_clientsize, $clientsize_text)
	;GUICtrlSetData($edit_executable, $process_name)
	WinActivate($gui_title)
	;CalculateClientOffset() !!!
EndFunc

Func NChairs()
	Local $chairs = Number(ControlGetText($gui_title, "", $combo_chairs))
	If ($chairs < 2) Or ($chairs > 10) Then
		Return 10
	EndIf
	Return $chairs()
EndFunc
