#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

; Script Start - Add your code below here

#include-once

#include <ComboConstants.au3>
#include <GUIConstantsEx.au3>
#include <Data.au3>

; Needed in the main message loop
; Needs to be initialited, otherwise it matches an undefined message
Global $button_symbols_OK= -1
Global $symbols_GUI = -1

Const $k_tooltip_betsizeinterpretationmethod = "Used by the Autoplayer to determine how to enter the " & @crlf & _
    "f$betsize amount into the betsize entry box on the casino " & @crlf & _
	"window. This is necessary, as bets and raises get " & @crlf & _
	"interpreted differently by different casinos." & @crlf & _
	"  1( Enter (f$betsize - currentbet - call)" & @crlf & _
    "  2) Enter (f$betsize - currentbet) & @crlf" & @crlf & _
    "  3( Enter f$betsize without any adjustment"

Const $k_tooltip_potmethod = "This is used to determine the appropriate site interpretation " & @crlf & _
    "for the contents of c0pot0:" & @crlf & _
    "  1) Common pot" & @crlf & _
    "  2) Total pot, includes current bets and side bets" & @crlf & _
    "  3( Largest of all pots (common pot and sidepots)"

Const $k_tooltip_cardscrapemethod = "Default: scraping first cardbacks, then nocard, then" & @crlf & _
    "cardfaces (or ranks + suits). Works for most casinos" & @crlf & _
	"optimal performance and also serves the case where" & @crlf & _
	"cards and cardbacks are at different locations" & @crlf & _
	"" & @crlf & _
	"1: scraping first card-faces, then cardbacks, finally" & @crlf & _
	"nocard. This method is necessary at casinos that" & @crlf & _
	"display additional cardbacks also for players that hold" & @crlf & _
	"face-cards."

Func CreateSymbolsGUI()
	Const $button_width = 150
	Const $left_position_labels = 10
	Local $y_position = 10
	Const $cell_height = 30
	Const $left_position_editables = $left_position_labels + $button_width + 10
	Local $combo_widt = 100

	$symbols_GUI = GUICreate("Symbol Editor", 280, 450)
	GUICtrlCreateLabel("table type", $left_position_labels, $y_position)
	Global $combo_table_type = GUICtrlCreateCombo("Poker Table", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "Lobby|Popup", "Poker Table")
	$y_position +=$cell_height
	;GUICtrlCreateLabel("sitename", $left_position_labels, $y_position)
	;GUICtrlCreateInput("", $left_position_editables, $y_position,$combo_widt)
	;$y_position +=$cell_height
	GUICtrlCreateLabel("network", $left_position_labels, $y_position, $combo_widt)
	Global $combo_network = GUICtrlCreateCombo("stars", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "party|fulltilt|ipoker|everest|ongame|boss|cereus|pacific|b2b|microgaming|cake|bodog", "")
	$y_position +=$cell_height
	GUICtrlCreateLabel("allinconfirmationmethod", $left_position_labels, $y_position)
	Global $combo_allinconfirmationmethod = GUICtrlCreateCombo("Nothing", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "Click Bet", "")
	$y_position +=$cell_height
	GUICtrlCreateLabel("betsizeinterpretationmethod", $left_position_labels, $y_position)
	Global $combo_betsizeinterpretationmethod = GUICtrlCreateCombo("3", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "2|1", "")
	GUICtrlSetTip($combo_betsizeinterpretationmethod, $k_tooltip_betsizeinterpretationmethod)
	$y_position +=$cell_height
	GUICtrlCreateLabel("potmethod", $left_position_labels, $y_position)
	Global $combo_potmethod = GUICtrlCreateCombo("1", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "2|3", "")
	GUICtrlSetTip($combo_potmethod, $k_tooltip_potmethod )
	$y_position +=$cell_height
	GUICtrlCreateLabel("betsizeselectionmethod", $left_position_labels, $y_position)
	Global $combo_betsizeselectionmethod = GUICtrlCreateCombo("Nothing", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "Sgl Click|Dbl Click|Triple Click|Click Drag", "")
	$y_position +=$cell_height
	GUICtrlCreateLabel("betsizedeletionmethod", $left_position_labels, $y_position)
	Global $combo_betsizedeletionmethod = GUICtrlCreateCombo("Nothing", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "Backspace|Delete", "Nothing")
	$y_position +=$cell_height
	GUICtrlCreateLabel("betsizeconfirmationmethod", $left_position_labels, $y_position)
	Global $combo_betsizeconfirmatrionmethod = GUICtrlCreateCombo("Enter", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "Click Bet", "")
	$y_position +=$cell_height
	GUICtrlCreateLabel("buttonclickmethod", $left_position_labels, $y_position)
	Global $combo_buttonclickmethod = GUICtrlCreateCombo("Single", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "Double", "")
	$y_position +=$cell_height
	GUICtrlCreateLabel("betpotmethod", $left_position_labels, $y_position)
	Global $combo_betpotmethod = GUICtrlCreateCombo("None", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "Raise", "")
	$y_position +=$cell_height
	GUICtrlCreateLabel("use_comma_instead_of_dot", $left_position_labels, $y_position)
	Global $combo_use_comma_instead_of_dot = GUICtrlCreateCombo("0", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "1", "")
	$y_position +=$cell_height
	GUICtrlCreateLabel("cardscrapemethod", $left_position_labels, $y_position)
	Global $combo_cardscrapemethod = GUICtrlCreateCombo("Default", $left_position_editables, $y_position, $combo_widt, $CBS_DROPDOWNLIST)
    GUICtrlSetData(-1, "1", "")
	GUICtrlSetTip($combo_cardscrapemethod, $k_tooltip_cardscrapemethod)
	$y_position +=$cell_height
	$y_position +=$cell_height
	$button_symbols_OK = GUICtrlCreateButton("OK", 65, $y_position, $button_width)
	GUISetIcon("OpenHoldem_EasyScraper.ico")
	GUISetState()
EndFunc

Func SymbolsGui_StoreSymbol($symbol, $value)
	$symbols_data[$index][0] = $symbol
	$symbols_data[$index][1] = $value
	$index += 1
EndFunc

Func SymbolsGui_StoreSelection()
	Global $index = 0
	Local $value = ControlGetText($symbols_GUI, "", $combo_table_type)
	If $value = "Lobby" Then
		SymbolsGui_StoreSymbol("s$islobby", 1)
	ElseIf $value = "Popup" Then
		SymbolsGui_StoreSymbol("s$ispopup", 1)
	EndIf
	$value = ControlGetText($symbols_GUI , "", $combo_network)
	$value = GUICtrlRead($combo_network)
	SymbolsGui_StoreSymbol("s$network", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_allinconfirmationmethod)
	SymbolsGui_StoreSymbol("s$allinconfirmationmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_betsizeinterpretationmethod)
	SymbolsGui_StoreSymbol("s$betsizeinterpretationmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_potmethod)
	SymbolsGui_StoreSymbol("s$potmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_betsizeselectionmethod)
	SymbolsGui_StoreSymbol("s$betsizeselectionmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_betsizedeletionmethod)
	SymbolsGui_StoreSymbol("s$betsizedeletionmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_betsizeconfirmatrionmethod)
	SymbolsGui_StoreSymbol("s$betsizeconfirmatrionmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_buttonclickmethod)
	SymbolsGui_StoreSymbol("s$buttonclickmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_betpotmethod)
	SymbolsGui_StoreSymbol("s$betpotmethod", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_use_comma_instead_of_dot)
	SymbolsGui_StoreSymbol("s$use_comma_instead_of_dot", $value)
	$value = ControlGetText($symbols_GUI , "", $combo_cardscrapemethod)
	SymbolsGui_StoreSymbol("s$cardscrapemethod", $value)
EndFunc
