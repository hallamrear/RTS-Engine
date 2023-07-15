// Editor.cpp : Defines the entry point for the application.

#include "pch.h"
#include "resource.h"
#include <Engine.h>
#include <Window.h>
#include <chrono>

#include <mini/ini.h>

HINSTANCE g_Instance = NULL;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    MainWindowWndProc(HWND, UINT, WPARAM, LPARAM);

using namespace Bennett;

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int argc = __argc;
    char** argv = __argv;

#ifdef _DEBUG
    if (!AttachConsole(ATTACH_PARENT_PROCESS))   // try to hijack existing console of command line
        AllocConsole();                           // or create your own.

    FILE* file = nullptr;
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
#endif

    g_Instance = hInstance;

    WindowDetails mainWindowDetails;
    LoadString(hInstance, IDS_APP_TITLE, mainWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_GAME, mainWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    mainWindowDetails.WindowStyles = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
    mainWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_APPWORKSPACE);
    mainWindowDetails.ClassDetails.WndProcCallback = MainWindowWndProc;
    mainWindowDetails.ClassDetails.Icon = IDI_GAME;
    mainWindowDetails.ClassDetails.SmallIcon = IDI_SMALL;
    mainWindowDetails.ClassDetails.MenuName = IDC_GAME;
    Window* mainWindow = Window::CreateWin32Window(mainWindowDetails);

    // Perform application initialization:
    if (!mainWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME));

    Engine* engine = CreateEngine(*mainWindow);

    if (!engine)
    {
        Log("Failed to create engine.", LOG_CRITICAL);
        return 0;
    }

    auto lTime = std::chrono::steady_clock::now();
    auto cTime = lTime;
    float dTime = 0.0f;

    MSG msg{};
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

    return (int)msg.wParam;
}

LRESULT CALLBACK MainWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
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