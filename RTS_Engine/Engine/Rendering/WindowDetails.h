#pragma once
#include <Defines/BennettDefines.h>
#include <string>

namespace Bennett
{
	class Window;

	struct BENNETT_ENGINE WindowDetails
	{

		struct BENNETT_ENGINE WindowWin32ClassDetails
		{
			CHAR ClassName[MAX_LOADSTRING] = "";
			INT MenuName = NULL;
			WNDPROC WndProcCallback = NULL;
			int Icon = NULL;
			int SmallIcon = NULL;
			HBRUSH BackgroundColour = 0;
			int AdditionalClassStyles = 0;

			WindowWin32ClassDetails() { };
		} ClassDetails;

		CHAR Title[MAX_LOADSTRING] = "";
		DWORD WindowStyles = 0;
		int X = CW_USEDEFAULT; int Y = CW_USEDEFAULT;
		int Width = CW_USEDEFAULT; int Height = CW_USEDEFAULT;
		Window* Parent = nullptr;
		LPVOID lpParam = nullptr;

		enum BENNETT_ENGINE WINDOW_SHOW_STATE
		{
			HIDDEN = SW_HIDE,
			NORMAL = SW_NORMAL,
			MINIMIZED = SW_SHOWMINIMIZED,
			MAXIMIZED = SW_SHOWMAXIMIZED,
			RESTORE = SW_RESTORE
		} ShowState = WINDOW_SHOW_STATE::NORMAL;
	};


}