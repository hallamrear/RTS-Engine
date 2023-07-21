#pragma once
#include "resource.h"
#include <WindowDetails.h>
#include <Window.h>

using namespace Bennett;

Window* CreateRenderWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK RenderWindowExtraDetailsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);