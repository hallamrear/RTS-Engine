#pragma once
#include "resource.h"

namespace Bennett
{
	class Engine;
}

inline static HINSTANCE g_Instance = NULL;
inline static Bennett::Engine* g_Engine = nullptr;
inline static char** g_Argv = nullptr;
inline static int g_Argc = -1;

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);