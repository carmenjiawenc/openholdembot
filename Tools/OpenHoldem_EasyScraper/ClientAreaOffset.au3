#include-once

#include <Data.au3>
#include <MiscFunctions.au3>

Global $client_area_offset_x
Global $client_area_offset_y

Func MouseCoordsToClientCoords(ByRef $position)
	ConsoleWriteLine(StringFormat("Absolute positions %i %i", $position[0], $position[1]))
	Local $tpoint = DllStructCreate("int X;int Y")
    DllStructSetData($tpoint, "X", $position[0])
    DllStructSetData($tpoint, "Y", $position[1])
    _WinAPI_ScreenToClient(ConnectedWindow(), $tpoint)
	$position[0] = DllStructGetData($tpoint, "X")
    $position[1] = DllStructGetData($tpoint, "Y")
	ConsoleWriteLine(StringFormat("Client positions %i %i", $position[0], $position[1]))
EndFunc