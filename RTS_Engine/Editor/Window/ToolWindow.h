#pragma once
#include "resource.h"
#include <Rendering/WindowDetails.h>
#include <Rendering/Window.h>

using namespace Bennett;

Window* CreateToolWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK ToolWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);