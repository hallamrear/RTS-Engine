#include "BennettPCH.h"
#include "Engine.h"
#include "Window.h"
#include <iostream>
#include <chrono>

using namespace Bennett;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int GenericMain(HINSTANCE hInstance, int argc, char** argv);

#ifdef _DEBUG

int main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	return GenericMain(hInstance, argc, argv);
}

#else

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	return GenericMain(hInstance, __argc, __argv);
}

#endif


int GenericMain(HINSTANCE hInstance, int argc, char** argv)
{
	WindowDetails details = WindowDetails();
	strcpy_s(details.Title, "Yet another engine but in Vulkan!");
	strcpy_s(details.ClassDetails.ClassName, "GameWindow");
	details.WindowStyles = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
	details.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_APPWORKSPACE);
	details.ClassDetails.WndProcCallback = WndProc;
	Window* mainWindow = Window::CreateWin32Window(details);

	Engine* engine = CreateEngine(*mainWindow);

	if (!engine)
	{
		Log("Engine Run finished. Engine deleting.", LOG_CRITICAL);
		return 0;
	}
	
	auto lTime = std::chrono::steady_clock::now();
	auto cTime = lTime;
	float dTime = 0.0f;

	MSG msg;
	while (engine->IsRunning())
	{
		if (PeekMessage(&msg, mainWindow->GetWindowHandle(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			cTime = std::chrono::steady_clock::now();
			dTime = (cTime - lTime).count();

			if (dTime > TIMESTEP_CAP)
				dTime = TIMESTEP_CAP;

			engine->ProcessInput(dTime);
			engine->Update(dTime);
			engine->Render();

			lTime = cTime;
		}
	}

	Log("Engine has finished running, it is now closing.", LOG_SAFE);
	delete engine;
	engine = nullptr;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}