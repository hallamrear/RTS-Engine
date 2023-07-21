#pragma once
#include "resource.h"
#include <WindowDetails.h>
#include <Window.h>

using namespace Bennett;

Window* CreatePropertiesWindow(HINSTANCE hInstance, Window* parentWindow);
LRESULT CALLBACK PropertiesWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);