// Editor.cpp : Defines the entry point for the application.

#include "pch.h"
#include "Editor.h"
#include <Window.h>
#include <Engine.h>
#include <Logger.h>
#include <chrono>

using namespace Bennett;

HINSTANCE g_Instance = NULL;
Engine* g_Engine = nullptr;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK MainWindowWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWindowWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

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
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
        AllocConsole();

    FILE* file = nullptr;
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
#endif

    g_Instance = hInstance;

    WindowDetails mainWindowDetails;
    LoadString(hInstance, IDS_APP_TITLE, mainWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_EDITOR, mainWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    mainWindowDetails.WindowStyles = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
    mainWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_APPWORKSPACE);
    mainWindowDetails.ClassDetails.WndProcCallback = MainWindowWndProc;
    mainWindowDetails.ClassDetails.WndProcCallback = Engine::WindowsCallbackProcedure;
    mainWindowDetails.ClassDetails.Icon = IDI_ICON1;
    mainWindowDetails.ClassDetails.SmallIcon = IDI_ICON1;
    mainWindowDetails.ClassDetails.MenuName = IDI_EDITOR;
    Window* mainWindow = Window::CreateWin32Window(mainWindowDetails);

    // Perform application initialization:
    if (!mainWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

    WindowDetails childWindowDetails;
    LoadString(hInstance, IDS_CHILDONEWINDOWTITLE, childWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_CHILDONECLASSNAME, childWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    childWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    childWindowDetails.X = 100; childWindowDetails.Y = 100;
    childWindowDetails.Width = 800; childWindowDetails.Height = 600;
    childWindowDetails.Parent = mainWindow;
    childWindowDetails.ClassDetails.WndProcCallback = ChildWindowWndProc;
    childWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_GRADIENTACTIVECAPTION);
    Window* childWindow = Window::CreateWin32Window(childWindowDetails);

    // Perform application initialization:
    if (!childWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

    WindowDetails childTwoWindowDetails;
    LoadString(hInstance, IDS_CHILDTWOWINDOWTITLE, childTwoWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_CHILDTWOCLASSNAME, childTwoWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    childTwoWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    childTwoWindowDetails.X = 700; childTwoWindowDetails.Y = 300;
    childTwoWindowDetails.Width = 300; childTwoWindowDetails.Height = 300;
    childTwoWindowDetails.Parent = mainWindow;
    childTwoWindowDetails.ClassDetails.WndProcCallback = ChildWindowWndProc;
    childTwoWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_GRADIENTACTIVECAPTION);
    Window* childTwoWindow = Window::CreateWin32Window(childTwoWindowDetails);

    // Perform application initialization:
    if (!childTwoWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITOR));
    
    g_Engine = Engine::CreateEngine(*childWindow);

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
            g_Engine->Render();

            lTime = cTime;
        }
    }

    Log("Engine has finished running, it is now closing.", LOG_SAFE);
    delete g_Engine;
    g_Engine = nullptr;

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
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
            case IDM_ABOUT:
                DialogBox(g_Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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


//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
LRESULT CALLBACK ChildWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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

    case WM_CLOSE:
        //Minimise a tool window when hitting the X button.
        PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
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
