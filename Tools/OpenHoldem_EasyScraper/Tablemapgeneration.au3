#include-once
#include <AccessorFunctions.au3>
#include <Array.au3>
#include <Data.au3>
#include <MiscFunctions.au3>

Global $tablemap_file

Func WriteTablemapData($data)
	AppendLineToEditLog($data)
	$data &= @CRLF
	FileWrite($tablemap_file, $data)
EndFunc   ;==>WriteTablemapData

Func DefaultLabel($task)
	Select
		Case $task = $task_button_fold
			Return 's$i0buttondefaultlabel      fold'
		Case $task = $task_button_check
			Return 's$i1buttondefaultlabel      check'
		Case $task = $task_button_call
			Return 's$i2buttondefaultlabel      call'
		Case $task = $task_button_minraise
			Return 's$i3buttondefaultlabel      raise'
		Case $task = $task_button_allin
			Return 's$i4buttondefaultlabel      allin'
		Case Else
			Return ""
	EndSelect
EndFunc   ;==>DefaultLabel

Func DataLineSize($name, $x, $y)
	Return StringFormat("%-18s%4s%5s", $name, $x, $y)
EndFunc   ;==>DataLineSize

Func DataLineSymbol($name, $value)
	Return StringFormat("%-28s %s", $name, $value)
EndFunc   ;==>DataLineSymbol

Func ReadFontMetaData($filename, ByRef $type, ByRef $width, ByRef $height, ByRef $ignore)
	; File format:
	;   * tXtype
	;   * width
	;   * height
	;   * optional "font already defined"
	local $font_file = FileOpen($filename)
	$type = FileReadLine($font_file)
	Local $next_line = FileReadLine($font_file)
	$width = Int(StringMid($next_line, 7))
	$next_line = FileReadLine($font_file)
	$height = Int(StringMid($next_line, 8))
	$next_line = FileReadLine($font_file)
	If $next_line = "font already defined" Then
		$ignore = True
	Else
		$ignore = False
	EndIf
	ConsoleWriteLine(StringFormat("Metadata %s %i %i %i", $type, $width, $height, $ignore))
EndFunc

Func SaveSizesToTablemap()
	WriteTablemapData("")
	WriteTablemapData("//")
	WriteTablemapData("// sizes")
	WriteTablemapData("//")
	WriteTablemapData("")
	Local $factor = (100 + SizeTolerance()) / 100
	WriteTablemapData(DataLineSize("z$clientsizemin", Int($targetsize[0] / $factor), Int($targetsize[1] / $factor)))
	WriteTablemapData(DataLineSize("z$clientsizemax", Int($targetsize[0] * $factor), Int($targetsize[1] * $factor)))
	WriteTablemapData(DataLineSize("z$targetsize", Int($targetsize[0]), Int($targetsize[1])))
EndFunc   ;==>SaveSizesToTablemap

Func SaveFontTypeToTablemap($font_file)
	ConsoleWriteLine(StringFormat("Font File %s", $font_file))
	ConsoleWriteLine(StringFormat("File exists %i", FileExists($font_file)))
	Local $type
	Local $dummy_width
	local $dummy_height
	local $ignore
	ReadFontMetaData($font_file, $type, $dummy_width, $dummy_height, $ignore)
	If $ignore then Return
	WriteTablemapData($type)
EndFunc

Func SaveAllFontTypesToTablemap()
	;_ArrayDisplay($font_file_for_task)
	SaveFontTypeToTablemap($font_file_for_task[$task_balances])
	SaveFontTypeToTablemap($font_file_for_task[$task_bets])
	SaveFontTypeToTablemap($font_file_for_task[$task_names])
EndFunc

Func SaveSymbolsToTablemap()
	WriteTablemapData("")
	WriteTablemapData("//")
	WriteTablemapData("// strings")
	WriteTablemapData("//")
	WriteTablemapData("")
	; Symbols: nchairs
	WriteTablemapData(DataLineSymbol("s$nchairs", NChairs()))
	; Symbols: process-name
	WriteTablemapData(DataLineSymbol("s$processname", ProcessName()))
	; Symbols: sitename
	WriteTablemapData(DataLineSymbol("s$sitename", SiteName()))
	; Symbols: titletext
	WriteTablemapData(DataLineSymbol("s$titletext", TitleText()))
	; Symbols: defazltlabels
	WriteTablemapData(DefaultLabel($task_button_fold))
	WriteTablemapData(DefaultLabel($task_button_check))
	WriteTablemapData(DefaultLabel($task_button_call))
	WriteTablemapData(DefaultLabel($task_button_minraise))
	WriteTablemapData(DefaultLabel($task_button_allin))
	For $i = 0 To (UBound($symbols_data, 1) - 1)
		Local $symbol = $symbols_data[$i][0]
		Local $value = $symbols_data[$i][1]
		If $symbol <> "" Then
			WriteTablemapData(DataLineSymbol($symbol, $value))
		EndIf
	Next
	SaveAllFontTypesToTablemap()
EndFunc   ;==>SaveSymbolsToTablemap

Func SaveRegionsToTablemap()
	WriteTablemapData("")
	WriteTablemapData("//")
	WriteTablemapData("// regions")
	WriteTablemapData("//")
	WriteTablemapData("")
	WriteTablemapData(RegionCollection_GetAll())
EndFunc   ;==>SaveRegionsToTablemap

Func SaveFontFileToTablemap($filename)
	if ($filename = "") then Return
	if not FileExists($filename) then
		ConsoleWriteError("Font file does not exist: " & $filename & @CRLF)
		Return
	EndIf
	local $font_file = FileOpen($filename)
	;!!!! check if not neeeded
	While True
		Local $next_line = FileReadLine($font_file)
		; EOF or entire file to be skipped
		If @error Then ExitLoop
		If StringLeft($next_line, 20) = "font already defined" Then ExitLoop
		; Meta-data to ve skippedt
		If StringLeft($next_line, 8) = "s$t0type" Then ContinueLoop
		If StringLeft($next_line, 8) = "s$t1type" Then ContinueLoop
		If StringLeft($next_line, 8) = "s$t2type:" Then ContinueLoop
		If StringLeft($next_line, 8) = "s$t3type" Then ContinueLoop
		If StringLeft($next_line, 5) = "width" Then ContinueLoop
		If StringLeft($next_line, 6) = "height" Then ContinueLoop
		If $next_line = "" Then ContinueLoop
		; regular font
		WriteTablemapData($next_line)
	WEnd
	FileClose($font_file)
EndFunc

Func SaveFontsToTablemap()
	WriteTablemapData("")
	WriteTablemapData("//")
	WriteTablemapData("// fonts")
	WriteTablemapData("//")
	WriteTablemapData("")
	;_ArrayDisplay($font_file_for_task)
	SaveFontFileToTablemap($font_file_for_task[$task_balances])
	SaveFontFileToTablemap($font_file_for_task[$task_bets])
	SaveFontFileToTablemap($font_file_for_task[$task_names])
	;SaveFontFileToTablemap("Fonts_DB\Balances\\Manualmode.txt")
EndFunc

Func SaveTablemap()
	If DigitsInString(TitleText()) Then
		MsgBox(48, "Error", _
				"Your title identifier contains digits, probably blind levels." & @CRLF & _
				"Please edit it in order to connect to more tables.")
		Return
	EndIf
	Local $filename = FileSaveDialog("Save Tablemap", @DesktopDir, "Tablemaps (*.tm)", 16, DefaultTablemapName())
	If @error Then
		Return
	EndIf
	ClearEditLog()
	$tablemap_file = FileOpen($filename, 2)
	WriteTablemapData(".osdb2")
	SaveSizesToTablemap()
	SaveSymbolsToTablemap()
	SaveRegionsToTablemap()
	SaveFontsToTablemap()
	; buttons
	FileClose($tablemap_file)
EndFunc   ;==>SaveTablemap