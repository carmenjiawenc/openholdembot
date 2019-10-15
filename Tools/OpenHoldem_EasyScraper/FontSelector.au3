#include <AccessorFunctions.au3>
#include <Array.au3>
#include <File.au3>
#include <GDIPlus.au3>
#include <GUIConstantsEx.au3>
#include <MiscFunctions.au3>
#include <WindowsConstants.au3>
#include <GuiButton.au3>

Const $k_spacing = 10
Const $k_button_width = 160
Const $k_button_height = 65
Const $k_cell_witdh = $k_button_width + $k_spacing
Const $k_cell_height = $k_button_height + $k_spacing
const $k_height_of_information_text = 3 * 15
const $n_columns = 4

; Array gets extended later
Dim $font_buttons[2] = [-2, -2]
Global $font_selector_GUI = -1

Func ButtonText($image_path)
	Local $drive, $directory, $filename, $extension
	_PathSplit($image_path, $drive, $directory, $filename, $extension)
	Return $filename
EndFunc

Func ImageButton($left, $top, $width, $height, $image_filename)
	_GDIPlus_Startup()
	; button text and image file path
	Local $button_text = ButtonText($image_filename)
	;local $image_path = "OpenHoldem_EasyScraper.ico";FontFileFullPath($button_text & ".bmp")
	local $image_path = FontFileFullPath($button_text & ".ico")
	ConsoleWriteLine(StringFormat("Image path %s", $image_path))
	; Determine image dimensions
	Local $image = _GDIPlus_ImageLoadFromFile($image_path)
	Local $image_width = _GDIPlus_ImageGetWidth($image)
	Local $image_height = _GDIPlus_ImageGetHeight($image)
	ConsoleWriteLine(StringFormat("Image size: %i %i", $image_width, $image_height))
	ConsoleWriteLine(StringFormat("Image exists %i", FileExists($image_path)))
	; Determine image position on button
	; Create image
	Const $DLL_icon_number = -1
	Local $image_left
	Local $image_top
	CalculateCenteredRectanglePosition($left, $top, $width, $height, $image_width, $image_height, _
		$image_left, $image_top, 0, -10)
	GUICtrlCreateIcon($image_path, $DLL_icon_number, $image_left, $image_top,	$image_width, $image_height)
	;GUICtrlCreateIcon("OpenHoldem_EasyScraper.ico", $DLL_icon_number, $left + 5, $top + 5,	$image_width, $image_height)
	;GUICtrlCreatePic($image_path, $left + 5, $top + 5)
	; Create text button
	Local $button = GUICtrlCreateButton($button_text, $left, $top, $width, $height, BitOr($WS_CLIPSIBLINGS, $BS_BOTTOM))
	_GDIPlus_Shutdown()
	Return $button
EndFunc

Func CreateButton($index, $filename)
	Local $x_slot = Mod($index, $n_columns)
	Local $y_slot = Int($index / $n_columns)
	Local $x_position = ($x_slot * $k_cell_witdh) + $k_spacing
	Local $y_position = ($y_slot * $k_cell_height) + $k_spacing + $k_height_of_information_text + $k_spacing
	;Local $button = GUICtrlCreateButton($filename, $x_position, $y_position, $k_button_width, $k_button_height)
	Local $button = ImageButton($x_position, $y_position, $k_button_width, $k_button_height, $filename)
	return $button
EndFunc

Func CalculateGUIHeight($fonts_directory)
	Local $file_list = _FileListToArray($fonts_directory, "*.ico", 1)
	Local $files_found = UBound($file_list)
	ConsoleWriteLine(StringFormat("Files found %i", $files_found))
	Local $button_rows_needed = Int(($files_found / $n_columns) + 0.99)
	Local $height = $k_spacing
	$height += $k_height_of_information_text
	$height += $k_spacing
	$height += $button_rows_needed * $k_cell_height
	ConsoleWriteLine(StringFormat("GUI height %i", $height))
	return $height
EndFunc

Func CreateFontSelector($fonts_directory)
	_GDIPlus_Startup()
	; Define working-dir for "Fonts_DB",
	; Save-file-dialog might have it changed
	FileChangeDir(@ScriptDir)
    $font_selector_GUI = GUICreate("Font Selection", 690, CalculateGUIHeight($fonts_directory))
	GUICtrlCreateLabel("Please choose the closest font.", 10, 10, 300, 12)
	GUICtrlCreateLabel("Size and shape matter, colours are not relevant.", 10, 25, 300, 12)
	GUICtrlCreateLabel("If you are lucky it might work.", 10,40, 300, 12)
	; will create a dialog box that when displayed is centered
	;MsgBox(0, "", FileExists($fonts_directory))
	Local $file_list = _FileListToArray($fonts_directory, "*.txt", 1)
	_ArraySort($file_list, False, 1)
	GUISetState(@SW_SHOW, $font_selector_GUI)
	For $i = 1 To (UBound($file_list) - 1)
		Local $next_button = CreateButton(($i-1), $file_list[$i])
		_ArrayAdd($font_buttons, $next_button)
	Next
	;GUICtrlSetImage(-1, "shell32.dll", 22)
	;MsgBox(0, "", FileExists(@DesktopDir & "\BL_brandi_526images.jpg"))
	;MsgBox(0, "", _GUICtrlButton_SetImage(-1, @DesktopDir & "\BL_brandi_526images.jpg"))
	GUISetIcon("OpenHoldem_EasyScraper.ico")
	GUISetState(@SW_SHOW) ; will display an empty dialog box
	_GDIPlus_Shutdown()
	; Run the GUI until the dialog is closed
EndFunc