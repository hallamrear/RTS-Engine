#pragma once
#include "resource.h"
#include <Rendering/WindowDetails.h>
#include <Rendering/Window.h>

using namespace Bennett;

Window* CreateHierarchyWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK HierarchyWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);