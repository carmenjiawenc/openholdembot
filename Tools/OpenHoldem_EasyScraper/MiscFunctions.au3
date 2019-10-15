#include-once

Func ConsoleWriteLine($data)
	$data &= @CRLF
	ConsoleWrite($data)
EndFunc

Func CalculateCenteredRectanglePosition($outer_region_left_x, $outer_region_top_y, _
	$outer_region_width,  $outer_region_height, $inner_region_width,  $inner_region_height, _
	ByRef $inner_left_x, ByRef $inner_top_y, $offset_x = 0, $offset_y = 0)
	; calculate relative inner position
	Local $relative_inner_left_x = Int((($outer_region_width -  $inner_region_width) / 2) +  $offset_x)
	Local $relative_inner_top_y = Int((($outer_region_height -  $inner_region_height) / 2) +  $offset_y)
	ConsoleWriteLine(StringFormat("Relative inner region position %i %i", $relative_inner_left_x, $relative_inner_top_y))
	; absolue positions
	$inner_left_x = $relative_inner_left_x + $outer_region_left_x
	$inner_top_y = $relative_inner_top_y + $outer_region_top_y
EndFunc

Func DigitsInString($title)
	Local $digit_found = StringInStr($title, "0") _
			Or StringInStr($title, "1") _
			Or StringInStr($title, "2") _
			Or StringInStr($title, "3") _
			Or StringInStr($title, "4") _
			Or StringInStr($title, "5") _
			Or StringInStr($title, "6") _
			Or StringInStr($title, "7") _
			Or StringInStr($title, "8") _
			Or StringInStr($title, "9")
	Return $digit_found
EndFunc   ;==>DigitsIntString

func ARGB_to_ABGR($colour_ARGB)
	ConsoleWriteLine(StringFormat("ARGB %s", Hex($colour_ARGB)))
	local $right = 8
	local $left = -8
	; extrqct ARGB
	local $blue = BitAND($colour_ARGB, 0xFF)
	$colour_ARGB = BitShift($colour_ARGB, $right)
	local $green = BitAND($colour_ARGB, 0xFF)
	$colour_ARGB = BitShift($colour_ARGB, $right)
	local $red = BitAND($colour_ARGB, 0xFF)
	$colour_ARGB = BitShift($colour_ARGB, $right)
	local $alpha = BitAND($colour_ARGB, 0xFF)
	; debug
	ConsoleWriteLine(StringFormat("Alpha %s", Hex($alpha)))
	ConsoleWriteLine(StringFormat("Blue  %s", Hex($blue)))
	ConsoleWriteLine(StringFormat("Green %s", Hex($green)))
	ConsoleWriteLine(StringFormat("Red   %s", Hex($red)))
	; construct ABGR
	Local $result = $alpha
	$result = BitShift($result, $left)
	$result = BitOR($result, $blue)
	$result = BitShift($result, $left)
	$result = BitOR($result, $green)
	$result = BitShift($result, $left)
	$result = BitOR($result, $red)
	; return
	ConsoleWriteLine(StringFormat("ABGR %s", Hex($result)))
	return $result
EndFunc