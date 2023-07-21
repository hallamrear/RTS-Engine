// Editor.cpp : Defines the entry point for the application.

#include "pch.h"
#include "Editor.h"
#include <Window.h>
#include <Engine.h>
#include <Logger.h>
#include <chrono>

#include "PropertiesWindow.h"
#include "ToolWindow.h"
#include "HierarchyWindow.h"
#include "RenderWindow.h"

using namespace Bennett;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK MainWindowWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    g_Argc = __argc;
    g_Argv = __argv;
    g_Instance = hInstance;

#ifdef _DEBUG
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
        AllocConsole();

    FILE* file = nullptr;
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
#endif

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITOR));

    WindowDetails mainWindowDetails;
    LoadString(hInstance, IDS_APP_TITLE, mainWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_EDITOR, mainWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    mainWindowDetails.WindowStyles = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
    mainWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_APPWORKSPACE);
    mainWindowDetails.ClassDetails.WndProcCallback = MainWindowWndProc;
    #ifdef _DEBUG
    mainWindowDetails.ShowState = WindowDetails::NORMAL;
    #else
    mainWindowDetails.ShowState = WindowDetails::MAXIMIZED;
    #endif
    mainWindowDetails.ClassDetails.Icon = IDI_FACE;
    mainWindowDetails.ClassDetails.SmallIcon = IDI_FACE;
    mainWindowDetails.ClassDetails.MenuName = IDC_EDITOR;
    Window* mainWindow = Window::CreateWin32Window(mainWindowDetails);
 
    // Perform application initialization:
    if (!mainWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

    Window* renderWindow = CreateRenderWindow(hInstance, mainWindow);
    if (!renderWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); return FALSE; }

    Window* propertiesWindow = CreatePropertiesWindow(hInstance, mainWindow);
    if (!propertiesWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); return FALSE; }

    Window* toolWindow = CreateToolWindow(hInstance, mainWindow);
    if (!toolWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); return FALSE; }

    Window* hierarchyWindow = CreateHierarchyWindow(hInstance, mainWindow);
    if (!hierarchyWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); return FALSE; }

    TileWindows(mainWindow->GetWindowHandle(), MDITILE_HORIZONTAL | MDITILE_VERTICAL, NULL, 0, NULL);

    g_Engine = Engine::CreateEngine(*renderWindow);
    if (!g_Engine)
    {
        Log("Engine Run finished. Engine deleting.", LOG_CRITICAL);
        return 0;
    }

    auto lTime = std::chrono::steady_clock::now();
    auto cTime = lTime;
    float dTime = 0.0f;

    MSG msg{};
    while (g_Engine->IsRunning())
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

            g_Engine->ProcessInput(dTime);
            g_Engine->Update(dTime);
            //g_Engine->Render();

            lTime = cTime;
        }
    }

    //Todo : proper cleanup of all windows.

    Log("Engine has finished running, it is now closing.", LOG_SAFE);
    delete g_Engine;
    g_Engine = nullptr;

    return (int)msg.wParam;
}

LRESULT CALLBACK MainWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        g_Engine->SetInFocus(false);
    } 
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        if (g_Engine->GetInFocus())
        {
            g_Engine->WindowsCallbackProcedure(hWnd, message, wParam, lParam);
        }
        else
        {
            Log("Non engine keyboard input", LOG_SAFE);
            //todo : potentially send the messages to the children rather than handle this way.
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case ID_ARRANGEWINDOWS_4SQUARES:
                TileWindows(hWnd, MDITILE_HORIZONTAL | MDITILE_VERTICAL, NULL, 0, NULL);
                break;

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


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
