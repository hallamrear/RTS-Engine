#pragma once
#include <WinUser.h>
#include <vector>

namespace Bennett
{
#define BENNETT_MOUSE_LEFT		VK_LBUTTON		//Left Mouse Button
#define BENNETT_MOUSE_MIDDLE	VK_MBUTTON		//Middle Mouse Button
#define BENNETT_MOUSE_RIGHT		VK_RBUTTON		//Right Mouse Button
#define BENNETT_MOUSE_XBUTTON1	VK_XBUTTON1		//X1 Mouse Button
#define BENNETT_MOUSE_XBUTTON2	VK_XBUTTON2		//X2 Mouse Button
#define BENNETT_KEY_CANCEL 		VK_CANCEL		//Control
#define BENNETT_KEY_BACK 		VK_BACK			//BACKSPACE key
#define BENNETT_KEY_TAB 		VK_TAB			//TAB key
#define BENNETT_KEY_CLEAR 		VK_CLEAR		//CLEAR key
#define BENNETT_KEY_RETURN 		VK_RETURN		//ENTER key
#define BENNETT_KEY_CONTROL		VK_CONTROL		//CTRL key
#define BENNETT_KEY_PAUSE 		VK_PAUSE		//PAUSE key
#define BENNETT_KEY_CAPITAL		VK_CAPITAL		//CAPS LOCK key
#define BENNETT_KEY_SPACE 		VK_SPACE		//SPACEBAR
#define BENNETT_KEY_PRIOR 		VK_PRIOR		//PAGE UP key
#define BENNETT_KEY_NEXT 		VK_NEXT			//PAGE DOWN key
#define BENNETT_KEY_END			VK_END			//END key
#define BENNETT_KEY_HOME 		VK_HOME			//HOME key
#define BENNETT_KEY_LEFT 		VK_LEFT			//LEFT ARROW key
#define BENNETT_KEY_UP 			VK_UP 			//UP ARROW key
#define BENNETT_KEY_RIGHT 		VK_RIGHT		//RIGHT ARROW key
#define BENNETT_KEY_DOWN 		VK_DOWN			//DOWN ARROW key
#define BENNETT_KEY_SELECT 		VK_SELECT		//SELECT key
#define BENNETT_KEY_PRINT 		VK_PRINT		//PRINT key
#define BENNETT_KEY_EXECUTE		VK_EXECUTE		//EXECUTE key
#define BENNETT_KEY_SNAPSHOT	VK_SNAPSHOT		//PRINT SCREEN key
#define BENNETT_KEY_INSERT 		VK_INSERT		//INS key
#define BENNETT_KEY_DELETE 		VK_DELETE		//DEL key
#define BENNETT_KEY_HELP 		VK_HELP			//HELP key
#define BENNETT_KEY_0			0x30			//Number 0
#define BENNETT_KEY_1			0x31			//Number 1
#define BENNETT_KEY_2			0x32			//Number 2
#define BENNETT_KEY_3			0x33			//Number 3
#define BENNETT_KEY_4			0x34			//Number 4
#define BENNETT_KEY_5			0x35			//Number 5
#define BENNETT_KEY_6			0x36			//Number 6
#define BENNETT_KEY_7			0x37			//Number 7
#define BENNETT_KEY_8			0x38			//Number 8
#define BENNETT_KEY_9			0x39			//Number 9
#define BENNETT_KEY_A			0x41			//Letter A
#define BENNETT_KEY_B			0x42			//Letter B
#define BENNETT_KEY_C			0x43			//Letter C
#define BENNETT_KEY_D			0x44			//Letter D
#define BENNETT_KEY_E			0x45			//Letter E
#define BENNETT_KEY_F			0x46			//Letter F
#define BENNETT_KEY_G			0x47			//Letter G
#define BENNETT_KEY_H			0x48			//Letter H
#define BENNETT_KEY_I			0x49			//Letter I
#define BENNETT_KEY_J			0x4A			//Letter J
#define BENNETT_KEY_K			0x4B			//Letter K
#define BENNETT_KEY_L			0x4C			//Letter L
#define BENNETT_KEY_M			0x4D			//Letter M
#define BENNETT_KEY_N			0x4E			//Letter N
#define BENNETT_KEY_O			0x4F			//Letter O
#define BENNETT_KEY_P			0x50			//Letter P
#define BENNETT_KEY_Q			0x51			//Letter Q
#define BENNETT_KEY_R			0x52			//Letter R
#define BENNETT_KEY_S			0x53			//Letter S
#define BENNETT_KEY_T			0x54			//Letter T
#define BENNETT_KEY_U			0x55			//Letter U
#define BENNETT_KEY_V			0x56			//Letter V
#define BENNETT_KEY_W			0x57			//Letter W
#define BENNETT_KEY_X			0x58			//Letter X
#define BENNETT_KEY_Y			0x59			//Letter Y
#define BENNETT_KEY_Z			0x5A			//Letter Z
#define BENNETT_KEY_LWIN 		VK_LWIN			//Left Windows key(Natural keyboard)
#define BENNETT_KEY_RWIN 		VK_RWIN			//Right Windows key(Natural keyboard)
#define BENNETT_KEY_F1			VK_F1			//Function Key F1
#define BENNETT_KEY_F2			VK_F2			//Function Key F2
#define BENNETT_KEY_F3			VK_F3			//Function Key F3
#define BENNETT_KEY_F4			VK_F4			//Function Key F4
#define BENNETT_KEY_F5			VK_F5			//Function Key F5
#define BENNETT_KEY_F6			VK_F6			//Function Key F6
#define BENNETT_KEY_F7			VK_F7			//Function Key F7
#define BENNETT_KEY_F8			VK_F8			//Function Key F8
#define BENNETT_KEY_F9			VK_F9			//Function Key F9
#define BENNETT_KEY_F10			VK_F10			//Function Key F10
#define BENNETT_KEY_F11			VK_F11			//Function Key F11
#define BENNETT_KEY_F12			VK_F12			//Function Key F12
#define BENNETT_KEY_TAB			VK_TAB			//Tab key
#define BENNETT_KEY_NUMPAD0		VK_NUMPAD0		//Numeric keypad 0 key
#define BENNETT_KEY_NUMPAD1		VK_NUMPAD1		//Numeric keypad 1 key
#define BENNETT_KEY_NUMPAD2		VK_NUMPAD2		//Numeric keypad 2 key
#define BENNETT_KEY_NUMPAD3		VK_NUMPAD3		//Numeric keypad 3 key
#define BENNETT_KEY_NUMPAD4		VK_NUMPAD4		//Numeric keypad 4 key
#define BENNETT_KEY_NUMPAD5		VK_NUMPAD5		//Numeric keypad 5 key
#define BENNETT_KEY_NUMPAD6		VK_NUMPAD6		//Numeric keypad 6 key
#define BENNETT_KEY_NUMPAD7		VK_NUMPAD7		//Numeric keypad 7 key
#define BENNETT_KEY_NUMPAD8		VK_NUMPAD8		//Numeric keypad 8 key
#define BENNETT_KEY_NUMPAD9		VK_NUMPAD9		//Numeric keypad 9 key
#define BENNETT_KEY_MULTIPLY	VK_MULTIPLY		//Multiply key
#define BENNETT_KEY_ADD			VK_ADD			//Add key
#define BENNETT_KEY_SEPARATOR	VK_SEPARATOR	//Separator key
#define BENNETT_KEY_SUBTRACT	VK_SUBTRACT		//Subtract key
#define BENNETT_KEY_DECIMAL		VK_DECIMAL		//Decimal key
#define BENNETT_KEY_DIVIDE		VK_DIVIDE		//Divide key
#define BENNETT_KEY_F1			VK_F1			//F1 key
#define BENNETT_KEY_F2			VK_F2			//F2 key
#define BENNETT_KEY_F3			VK_F3			//F3 key
#define BENNETT_KEY_F4			VK_F4			//F4 key
#define BENNETT_KEY_F5			VK_F5			//F5 key
#define BENNETT_KEY_F6			VK_F6			//F6 key
#define BENNETT_KEY_F7			VK_F7			//F7 key
#define BENNETT_KEY_F8			VK_F8			//F8 key
#define BENNETT_KEY_F9			VK_F9			//F9 key
#define BENNETT_KEY_F10			VK_F10			//F10 key
#define BENNETT_KEY_F11			VK_F11			//F11 key
#define BENNETT_KEY_F12			VK_F12			//F12 key
#define BENNETT_KEY_F13			VK_F13			//F13 key
#define BENNETT_KEY_F14			VK_F14			//F14 key
#define BENNETT_KEY_F15			VK_F15			//F15 key
#define BENNETT_KEY_F16			VK_F16			//F16 key
#define BENNETT_KEY_F17			VK_F17			//F17 key
#define BENNETT_KEY_F18			VK_F18			//F18 key
#define BENNETT_KEY_F19			VK_F19			//F19 key
#define BENNETT_KEY_F20			VK_F20			//F20 key
#define BENNETT_KEY_F21			VK_F21			//F21 key
#define BENNETT_KEY_F22			VK_F22			//F22 key
#define BENNETT_KEY_F23			VK_F23			//F23 key
#define BENNETT_KEY_F24			VK_F24			//F24 key
#define BENNETT_KEYVKUMLOCK		VK_NUMLOCK		//NUM LOCK key
#define BENNETT_KEY_SCROLL	    VK_SCROLL 		//SCROLL LOCK key
#define BENNETT_KEY_LSHIFT	    VK_LSHIFT 		//Left SHIFT key
#define BENNETT_KEY_RSHIFT	    VK_RSHIFT 		//Right SHIFT key
#define BENNETT_KEY_LCONTROL    VK_LCONTROL 	//Left CONTROL key
#define BENNETT_KEY_RCONTROL    VK_RCONTROL 	//Right CONTROL key
#define BENNETT_KEY_LALT 	    VK_LMENU 		//Left ALT key
#define BENNETT_KEY_RALT 	    VK_RMENU 		//Right ALT key

	static std::unordered_map<int, const char*> INPUT_STRINGS =
	{
		std::pair<int, const char*>(BENNETT_MOUSE_LEFT	   	, "Left Mouse Button"),
		std::pair<int, const char*>(BENNETT_MOUSE_MIDDLE	, "Middle Mouse Button"),
		std::pair<int, const char*>(BENNETT_MOUSE_RIGHT		, "Right Mouse Button"),
		std::pair<int, const char*>(BENNETT_MOUSE_XBUTTON1	, "X1 Mouse Button"),
		std::pair<int, const char*>(BENNETT_MOUSE_XBUTTON2	, "X2 Mouse Button"),
		std::pair<int, const char*>(BENNETT_KEY_CANCEL 		, "Control"),
		std::pair<int, const char*>(BENNETT_KEY_BACK 		, "BACKSPACE key"),
		std::pair<int, const char*>(BENNETT_KEY_TAB 		, "TAB key"),
		std::pair<int, const char*>(BENNETT_KEY_CLEAR 		, "CLEAR key"),
		std::pair<int, const char*>(BENNETT_KEY_RETURN 		, "ENTER key"),
		std::pair<int, const char*>(BENNETT_KEY_CONTROL		, "CTRL key"),
		std::pair<int, const char*>(BENNETT_KEY_PAUSE 		, "PAUSE key"),
		std::pair<int, const char*>(BENNETT_KEY_CAPITAL		, "CAPS LOCK key"),
		std::pair<int, const char*>(BENNETT_KEY_SPACE 		, "SPACEBAR"),
		std::pair<int, const char*>(BENNETT_KEY_PRIOR 		, "PAGE UP key"),
		std::pair<int, const char*>(BENNETT_KEY_NEXT 		, "PAGE DOWN key"),
		std::pair<int, const char*>(BENNETT_KEY_END			, "END key"),
		std::pair<int, const char*>(BENNETT_KEY_HOME 		, "HOME key"),
		std::pair<int, const char*>(BENNETT_KEY_LEFT 		, "LEFT ARROW key"),
		std::pair<int, const char*>(BENNETT_KEY_UP 			, "UP ARROW key"),
		std::pair<int, const char*>(BENNETT_KEY_RIGHT 		, "RIGHT ARROW key"),
		std::pair<int, const char*>(BENNETT_KEY_DOWN 		, "DOWN ARROW key"),
		std::pair<int, const char*>(BENNETT_KEY_SELECT 		, "SELECT key"),
		std::pair<int, const char*>(BENNETT_KEY_PRINT 		, "PRINT key"),
		std::pair<int, const char*>(BENNETT_KEY_EXECUTE		, "EXECUTE key"),
		std::pair<int, const char*>(BENNETT_KEY_SNAPSHOT	, "PRINT SCREEN key"),
		std::pair<int, const char*>(BENNETT_KEY_INSERT 		, "INS key"),
		std::pair<int, const char*>(BENNETT_KEY_DELETE 		, "DEL key"),
		std::pair<int, const char*>(BENNETT_KEY_HELP 		, "HELP key"),
		std::pair<int, const char*>(BENNETT_KEY_0			, "Number 0"),
		std::pair<int, const char*>(BENNETT_KEY_1			, "Number 1"),
		std::pair<int, const char*>(BENNETT_KEY_2			, "Number 2"),
		std::pair<int, const char*>(BENNETT_KEY_3			, "Number 3"),
		std::pair<int, const char*>(BENNETT_KEY_4			, "Number 4"),
		std::pair<int, const char*>(BENNETT_KEY_5			, "Number 5"),
		std::pair<int, const char*>(BENNETT_KEY_6			, "Number 6"),
		std::pair<int, const char*>(BENNETT_KEY_7			, "Number 7"),
		std::pair<int, const char*>(BENNETT_KEY_8			, "Number 8"),
		std::pair<int, const char*>(BENNETT_KEY_9			, "Number 9"),
		std::pair<int, const char*>(BENNETT_KEY_A			, "Letter A"),
		std::pair<int, const char*>(BENNETT_KEY_B			, "Letter B"),
		std::pair<int, const char*>(BENNETT_KEY_C			, "Letter C"),
		std::pair<int, const char*>(BENNETT_KEY_D			, "Letter D"),
		std::pair<int, const char*>(BENNETT_KEY_E			, "Letter E"),
		std::pair<int, const char*>(BENNETT_KEY_F			, "Letter F"),
		std::pair<int, const char*>(BENNETT_KEY_G			, "Letter G"),
		std::pair<int, const char*>(BENNETT_KEY_H			, "Letter H"),
		std::pair<int, const char*>(BENNETT_KEY_I			, "Letter I"),
		std::pair<int, const char*>(BENNETT_KEY_J			, "Letter J"),
		std::pair<int, const char*>(BENNETT_KEY_K			, "Letter K"),
		std::pair<int, const char*>(BENNETT_KEY_L			, "Letter L"),
		std::pair<int, const char*>(BENNETT_KEY_M			, "Letter M"),
		std::pair<int, const char*>(BENNETT_KEY_N			, "Letter N"),
		std::pair<int, const char*>(BENNETT_KEY_O			, "Letter O"),
		std::pair<int, const char*>(BENNETT_KEY_P			, "Letter P"),
		std::pair<int, const char*>(BENNETT_KEY_Q			, "Letter Q"),
		std::pair<int, const char*>(BENNETT_KEY_R			, "Letter R"),
		std::pair<int, const char*>(BENNETT_KEY_S			, "Letter S"),
		std::pair<int, const char*>(BENNETT_KEY_T			, "Letter T"),
		std::pair<int, const char*>(BENNETT_KEY_U			, "Letter U"),
		std::pair<int, const char*>(BENNETT_KEY_V			, "Letter V"),
		std::pair<int, const char*>(BENNETT_KEY_W			, "Letter W"),
		std::pair<int, const char*>(BENNETT_KEY_X			, "Letter X"),
		std::pair<int, const char*>(BENNETT_KEY_Y			, "Letter Y"),
		std::pair<int, const char*>(BENNETT_KEY_Z			, "Letter Z"),
		std::pair<int, const char*>(BENNETT_KEY_LWIN 		, "Left Windows key"),
		std::pair<int, const char*>(BENNETT_KEY_RWIN 		, "Right Windows key"),
		std::pair<int, const char*>(BENNETT_KEY_F1			, "Function Key F1"),
		std::pair<int, const char*>(BENNETT_KEY_F2			, "Function Key F2"),
		std::pair<int, const char*>(BENNETT_KEY_F3			, "Function Key F3"),
		std::pair<int, const char*>(BENNETT_KEY_F4			, "Function Key F4"),
		std::pair<int, const char*>(BENNETT_KEY_F5			, "Function Key F5"),
		std::pair<int, const char*>(BENNETT_KEY_F6			, "Function Key F6"),
		std::pair<int, const char*>(BENNETT_KEY_F7			, "Function Key F7"),
		std::pair<int, const char*>(BENNETT_KEY_F8			, "Function Key F8"),
		std::pair<int, const char*>(BENNETT_KEY_F9			, "Function Key F9"),
		std::pair<int, const char*>(BENNETT_KEY_F10			, "Function Key F10"),
		std::pair<int, const char*>(BENNETT_KEY_F11			, "Function Key F11"),
		std::pair<int, const char*>(BENNETT_KEY_F12			, "Function Key F12"),
		std::pair<int, const char*>(BENNETT_KEY_TAB			, "Tab key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD0		, "Numeric keypad 0 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD1		, "Numeric keypad 1 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD2		, "Numeric keypad 2 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD3		, "Numeric keypad 3 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD4		, "Numeric keypad 4 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD5		, "Numeric keypad 5 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD6		, "Numeric keypad 6 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD7		, "Numeric keypad 7 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD8		, "Numeric keypad 8 key"),
		std::pair<int, const char*>(BENNETT_KEY_NUMPAD9		, "Numeric keypad 9 key"),
		std::pair<int, const char*>(BENNETT_KEY_MULTIPLY	, "Multiply key"),
		std::pair<int, const char*>(BENNETT_KEY_ADD			, "Add key"),
		std::pair<int, const char*>(BENNETT_KEY_SEPARATOR	, "Separator key"),
		std::pair<int, const char*>(BENNETT_KEY_SUBTRACT	, "Subtract key"),
		std::pair<int, const char*>(BENNETT_KEY_DECIMAL		, "Decimal key"),
		std::pair<int, const char*>(BENNETT_KEY_DIVIDE		, "Divide key"),
		std::pair<int, const char*>(BENNETT_KEY_F1			, "F1 key"),
		std::pair<int, const char*>(BENNETT_KEY_F2			, "F2 key"),
		std::pair<int, const char*>(BENNETT_KEY_F3			, "F3 key"),
		std::pair<int, const char*>(BENNETT_KEY_F4			, "F4 key"),
		std::pair<int, const char*>(BENNETT_KEY_F5			, "F5 key"),
		std::pair<int, const char*>(BENNETT_KEY_F6			, "F6 key"),
		std::pair<int, const char*>(BENNETT_KEY_F7			, "F7 key"),
		std::pair<int, const char*>(BENNETT_KEY_F8			, "F8 key"),
		std::pair<int, const char*>(BENNETT_KEY_F9			, "F9 key"),
		std::pair<int, const char*>(BENNETT_KEY_F10			, "F10 key"),
		std::pair<int, const char*>(BENNETT_KEY_F11			, "F11 key"),
		std::pair<int, const char*>(BENNETT_KEY_F12			, "F12 key"),
		std::pair<int, const char*>(BENNETT_KEY_F13			, "F13 key"),
		std::pair<int, const char*>(BENNETT_KEY_F14			, "F14 key"),
		std::pair<int, const char*>(BENNETT_KEY_F15			, "F15 key"),
		std::pair<int, const char*>(BENNETT_KEY_F16			, "F16 key"),
		std::pair<int, const char*>(BENNETT_KEY_F17			, "F17 key"),
		std::pair<int, const char*>(BENNETT_KEY_F18			, "F18 key"),
		std::pair<int, const char*>(BENNETT_KEY_F19			, "F19 key"),
		std::pair<int, const char*>(BENNETT_KEY_F20			, "F20 key"),
		std::pair<int, const char*>(BENNETT_KEY_F21			, "F21 key"),
		std::pair<int, const char*>(BENNETT_KEY_F22			, "F22 key"),
		std::pair<int, const char*>(BENNETT_KEY_F23			, "F23 key"),
		std::pair<int, const char*>(BENNETT_KEY_F24			, "F24 key"),
		std::pair<int, const char*>(BENNETT_KEYVKUMLOCK		, "NUM LOCK key"),
		std::pair<int, const char*>(BENNETT_KEY_SCROLL	    , "SCROLL LOCK key"),
		std::pair<int, const char*>(BENNETT_KEY_LSHIFT	    , "Left SHIFT key"),
		std::pair<int, const char*>(BENNETT_KEY_RSHIFT	    , "Right SHIFT key"),
		std::pair<int, const char*>(BENNETT_KEY_LCONTROL    , "Left CONTROL key"),
		std::pair<int, const char*>(BENNETT_KEY_RCONTROL    , "Right CONTROL key"),
		std::pair<int, const char*>(BENNETT_KEY_LALT 	    , "Left ALT key"),
		std::pair<int, const char*>(BENNETT_KEY_RALT 	    , "Right ALT key")
	};
}