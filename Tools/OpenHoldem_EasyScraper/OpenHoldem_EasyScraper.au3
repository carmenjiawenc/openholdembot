#region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Icon=OpenHoldem_EasyScraper.ico
#AutoIt3Wrapper_Res_LegalCopyright=OpenHoldem EasyScraper Licensed under GPL v3
#AutoIt3Wrapper_Run_Tidy=y
#AutoIt3Wrapper_Run_Obfuscator=y
#endregion ;**** Directives created by AutoIt3Wrapper_GUI ****
#cs ----------------------------------------------------------------------------
	
	AutoIt Version: 3.3.8.1
	Author:         myName
	
	Script Function:
	Template AutoIt script.
	
#ce ----------------------------------------------------------------------------

; Script Start - Add your code below

#include <Data.au3>
#include <MainGUI.au3>
#include <Array.au3>
#include <FontSelector.au3>
#include <GUIConstants.au3>
#include <GUIConstantsEx.au3>
#include <MiscFunctions.au3>
#include <Mouse.au3>
#include <SymbolsGUI.au3>
#include <Tablemapgeneration.au3>
#include <WinAPI.au3>
#include <WindowsConstants.au3>

AutoItSetOption("GUICloseOnESC", 0)
AutoItSetOption("MustDeclareVars", 1)
RemeberMouseSpeed()
CreateGUI()
HotKeySet("{F5}", "DataCollection")
WorkerLoop()
GUIDelete()
FileDelete($preview_temp_filename)
ResetMouseSpeed()

Func ConnectIfNotConnected()
	If ConnectedWindow() <= 0 Then
		Connect()
	ElseIf Not WinExists(ConnectedWindow()) Then
		Connect()
	EndIf
EndFunc   ;==>ConnectIfNotConnected

Func StartNewTask($new_task)
	ConnectIfNotConnected()
	$current_task = $new_task
	$mouseclicks_collected = 0
	$index_regions_for_current_task = 0
	SetSlowestMouseSpeed()
EndFunc   ;==>StartNewTask

Func WorkerLoop()
	Static $heartbeat_counter = 0
	While True
		$heartbeat_counter += 1
		Local $msg = GUIGetMsg(1)
		; advanced
		; 1 = returns an array containing the event and extended information.
		; https://www.autoitscript.com/wiki/Managing_Multiple_GUIs
		If $msg[1] = $main_GUI Then
			Select
				Case $msg[0] = $GUI_EVENT_CLOSE
					Return
				Case $msg[0] = $button_connect
					Connect()
				Case $msg[0] = $button_about
					ShowLicense()
				Case $msg[0] = $button_symbols
					CreateSymbolsGUI()
				Case $msg[0] = $button_seated
					InstructToCollectPixels("seated")
					StartNewTask($task_seated)
				Case $msg[0] = $button_active
					InstructToCollectPixels("active")
					StartNewTask($task_active)
				Case $msg[0] = $button_dealer
					InstructToCollectPixels("dealer")
					StartNewTask($task_dealer)
				Case $msg[0] = $button_cardback
					InstructToCollectPixels("cardback")
					StartNewTask($task_cardbacks)
				Case $msg[0] = $button_nocard
					InstructToCollectNoCards()
					StartNewTask($task_nocard)
				Case $msg[0] = $button_bets
					CreateFontSelector("Fonts_DB\Bets\")
					StartNewTask($task_bets)
				Case $msg[0] = $button_balances
					CreateFontSelector("Fonts_DB\Balances\")
					StartNewTask($task_balances)
				Case $msg[0] = $button_names
					CreateFontSelector("Fonts_DB\Names\")
					StartNewTask($task_names)
				Case $msg[0] = $button_buttons
					; Instruct how to do the first button.
					; More instructions follow later
					InstructToCollectButton("fold")
					StartNewTask($task_button_fold)
				Case $msg[0] = $button_save
					SaveTablemap()
			EndSelect
		ElseIf $msg[1] = $symbols_GUI Then
			Select
				Case $msg[0] = $GUI_EVENT_CLOSE
					GUIDelete($symbols_GUI)
				Case $msg[0] = $button_symbols_OK
					SymbolsGui_StoreSelection()
					GUIDelete($symbols_GUI)
			EndSelect
		ElseIf $msg[1] = $font_selector_GUI Then
			; Fonts GUI
			Switch $msg[0]
				Case $GUI_EVENT_CLOSE
					; Do nothing
					; user must click a button, even if "None"
					GUIDelete($font_selector_GUI) ;!!!!!!!!!!!!!!
				Case 0
					; workaround against bad button data
					ContinueLoop
			EndSwitch
			; Fonts-buttons
			For $i = 1 To (UBound($font_buttons) - 1)
				If $msg[0] = $font_buttons[$i] Then
					Local $font_file = GUICtrlRead($font_buttons[$i]) & ".txt"
					ConsoleWriteLine("Selected font file " & FontFileFullPath($font_file))
					$font_file_for_task[$current_task] = FontFileFullPath($font_file)
					;!!!ExtractInfoFromFontFile(FontFileFullPath($font_file))
					GUIDelete($font_selector_GUI)
					; Continue with next step
					InstructToCollectBetsBalancesNames()
					ExitLoop
				EndIf
			Next
		EndIf
		; Do certain tasks not on every heartbet
		If Mod($heartbeat_counter, 5) == 0 Then
			UpdateColourDisplay()
			UpdatePreviewDisplay()
			KeepTablePosition()
		EndIf
		; Usually a tight loop like the one shown would send the CPU to 100% - fortunately the GUIGetMsg function automatically idles the CPU when there are no events waiting. Do not put a manual sleep in the loop for fear of stressing the CPU - this will only cause the GUI to become unresponsive.
		Sleep(50);!!!!!!!!!
	WEnd
EndFunc   ;==>WorkerLoop

Func KeepTablePosition()
	Local $position = WinGetPos(ConnectedWindow())
	If (ConnectedWindow() = 0) Then
		Return
	EndIf
	If ($position[0] = 0) And ($position[1] = 0) Then
		Return
	EndIf
	WinMove(ConnectedWindow(), "", 0, 0)
EndFunc   ;==>KeepTablePosition

Func DataRequiredForNextItem()
	If ($current_task = $task_none) Then
		; handle negative index ($task_none = -1)
		; and the situation where we don't want to act on collected data
		Return 999999
	EndIf
	Return $coords_required_per_task[$current_task]
EndFunc   ;==>DataRequiredForNextItem

Func EnoughDataForNextItem()
	If ($mouseclicks_collected = DataRequiredForNextItem()) Then
		Return True
	EndIf
	Return False
EndFunc   ;==>EnoughDataForNextItem

Func DataCollection()
	Local $position = MouseGetPos()
	Local $colour = argb_to_abgr(PixelGetColor($position[0], $position[1]))
	MouseCoordsToClientCoords($position)
	_ArrayPush($current_region_data, $position[0])
	_ArrayPush($current_region_data, $position[1])
	_ArrayPush($current_region_data, $colour)
	$mouseclicks_collected += 1
	If EnoughDataForNextItem() Then
		CreateRegion()
		$mouseclicks_collected = 0
	EndIf
EndFunc   ;==>DataCollection

Func NameOfCurrentRegion()
	Local $result = "r$"
	Local $partial_region_name = $partial_region_name_for_task[$current_task]
	If ($partial_region_name == "button") Then
		$result &= "i"
	Else
		; Player
		$result &= "p"
	EndIf
	If $current_task = $task_nocard Then
		; Example: r$p0cardface2nocard
		Local Const $cards_per_player = 2
		Local $player_index = Int($index_regions_for_current_task / $cards_per_player)
		Local $card_index = Mod($index_regions_for_current_task, $cards_per_player)
		$result &= ($player_index & "cardface" & $card_index)
	Else
		; Example: r$p2balance
		$result &= $index_regions_for_current_task
	EndIf
	$result &= $partial_region_name
	Return $result
EndFunc   ;==>NameOfCurrentRegion

Func Radius()
	If $current_task = $task_active Then
		Return (0 - $default_colour_radius)
	EndIf
	Return $default_colour_radius
EndFunc   ;==>Radius

Func RegionTransform()
	Local $result
	Select
		Case $current_task = $task_balances
			$result = StringFormat("%4i T0", Radius())
		Case $current_task = $task_bets
			$result = StringFormat("%4i T1", Radius())
		Case $current_task = $task_names
			$result = StringFormat("%4i T2", Radius())
			;Case $current_task = $task_ !!!! caeds
			;$result = StringFormat("%4i T2", Radius())
		Case Else
			$result = StringFormat("%4i C", Radius())
	EndSelect
	Return $result
EndFunc   ;==>RegionTransform

Func CreateColourTransformRegion()
	Local $x = $current_region_data[6]
	Local $y = $current_region_data[7]
	Local $colour = $current_region_data[8]
	Local $coords_and_colour = StringFormat("%i %i %i %i %s %s", _
			$x, $y, $x, $y, Hex($colour), RegionTransform())
	RegionCollection_Add(NameOfCurrentRegion(), $coords_and_colour)
EndFunc   ;==>CreateColourTransformRegion

Func CreateBetBalanceNameRegion()
	Local $x = $current_region_data[3]
	Local $y = $current_region_data[4]
	; creating a reasonable region of size 80 x 12
	Local $x_left = $x - 40
	Local $x_right = $x + 40
	Local $y_top = $y - 6
	Local $y_bottom = $y + 6
	Local $colour = $current_region_data[8]
	Local $coords_and_colour = StringFormat("%i %i %i %i %s %s", _
			$x_left, $y_top, $x_right, $y_bottom, Hex($colour), RegionTransform())
	RegionCollection_Add(NameOfCurrentRegion(), $coords_and_colour)
EndFunc   ;==>CreateBetBalanceNameRegion

Func CreateButtonRegion()
	; button-region
	Local $x_left = $current_region_data[0]
	Local $y_top = $current_region_data[1]
	Local $x_right = $current_region_data[3]
	Local $y_bottom = $current_region_data[4]
	Local $coords_and_transform = StringFormat("%i %i %i %i 0 0 N", _
			$x_left, $y_top, $x_right, $y_bottom)
	Local $button_name = NameOfCurrentRegion()
	RegionCollection_Add($button_name, $coords_and_transform)
	; button-state
	$x_left = $current_region_data[6]
	$y_top = $current_region_data[7]
	Local $colour = $current_region_data[8]
	Local $coords_and_transform = StringFormat("%i %i %i %i %s %s", _
			$x_left, $y_top, $x_left, $y_top, Hex($colour), RegionTransform())
	Local $state_name = StringLeft($button_name, 4) & "state"
	RegionCollection_Add($state_name, $coords_and_transform)
	$current_task += 1
	InstructToCollectNextButton()
EndFunc   ;==>CreateButtonRegion

Func MarkTaskAsFinishedAfterEnoughRegions()
	Local $regions_required
	If $partial_region_name_for_task[$current_task] == "button" Then
		$regions_required = $buttons_to_be_collected
	ElseIf $current_task = $task_nocard Then
		$regions_required = 2 * NChairs()
	Else
		$regions_required = NChairs()
	EndIf
	If $index_regions_for_current_task >= $regions_required Then
		$index_regions_for_current_task = 0
		$current_task = $task_none
		ResetMouseSpeed()
	EndIf
EndFunc   ;==>MarkTaskAsFinishedAfterEnoughRegions

Func CreateRegion()
	If DataRequiredForNextItem() = 1 Then
		CreateColourTransformRegion()
	ElseIf DataRequiredForNextItem() = 2 Then
		CreateBetBalanceNameRegion()
	Else
		CreateButtonRegion()
	EndIf
	$index_regions_for_current_task += 1
	MarkTaskAsFinishedAfterEnoughRegions()
EndFunc   ;==>CreateRegion
