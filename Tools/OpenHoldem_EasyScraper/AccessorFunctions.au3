#include-once
#include <Data.au3>
#include <MiscFunctions.au3>

Func DefaultTablemapName()
	Local $name = SiteName() & ".tm"
	Return $name
EndFunc   ;==>DefaultTablemapName

Func FontFileFullPath($font_file)
	Local $path = @ScriptDir & "\Fonts_DB\"
	$path &= $partial_region_name_for_task[$current_task]
	$path &= "s\"
	$path &= $font_file
	ConsoleWriteLine("FontFileFullPath: " & $path)
	Return $path
EndFunc

Func ProcessName()
	Local $processname ;!!!= ControlGetText($gui_title, "", $edit_executable)
	Return $processname
EndFunc   ;==>ProcessName

Func SiteName()
	Local $process = ProcessName()
	Local $new_length = StringLen($process) - StringLen(".exe")
	Local $result = StringLeft($process, $new_length) & "_" & NChairs() & "max"
	Return $result
EndFunc   ;==>SiteName

Func SizeTolerance()
	Local $tolerance = 3;Number(ControlGetText($gui_title, "", $combo_size_tolerance))
	If ($tolerance < 0) Then
		Return 0
	EndIf
	If ($tolerance > 40) Then
		Return 40
	EndIf
	Return $tolerance
EndFunc   ;==>SizeTolerance

Func TitleText()
	Return ControlGetText($gui_title, "", $edit_table_identifier)
EndFunc   ;==>TitleText