#pragma once
#include "resource.h"
#include <WindowDetails.h>
#include <Window.h>

using namespace Bennett;

Window* CreateHierarchyWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK HierarchyWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);