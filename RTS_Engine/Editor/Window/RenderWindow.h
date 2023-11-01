#pragma once
#include "resource.h"
#include <Rendering/WindowDetails.h>
#include <Rendering/Window.h>

using namespace Bennett;

Window* CreateRenderWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK RenderWindowExtraDetailsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);