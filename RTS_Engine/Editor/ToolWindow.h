#pragma once
#include "resource.h"
#include <WindowDetails.h>
#include <Window.h>

using namespace Bennett;

Window* CreateToolWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK ToolWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);