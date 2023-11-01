#pragma once
#include "resource.h"
#include <Rendering/WindowDetails.h>
#include <Rendering/Window.h>

using namespace Bennett;

Window* CreatePropertiesWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK PropertiesWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);