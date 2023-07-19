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
char** g_Argv = nullptr;
int g_Argc = -1;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK MainWindowWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PropertiesWindowWndProc(HWND, UINT, WPARAM, LPARAM);
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
    mainWindowDetails.ShowState = WindowDetails::MAXIMIZED;
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

    WindowDetails gameWindowDetails;
    LoadString(hInstance, IDS_GAME_WINDOW_TITLE, gameWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_GAME_WINDOW, gameWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    gameWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    gameWindowDetails.X = 100; gameWindowDetails.Y = 100;
    gameWindowDetails.Width = 800; gameWindowDetails.Height = 600;
    gameWindowDetails.Parent = mainWindow;
    gameWindowDetails.ClassDetails.Icon = IDI_PICKAXE;
    gameWindowDetails.ClassDetails.SmallIcon = IDI_PICKAXE;
    gameWindowDetails.ClassDetails.WndProcCallback = Engine::WindowsCallbackProcedure;
    gameWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_GRADIENTACTIVECAPTION);
    Window* gameWindow = Window::CreateWin32Window(gameWindowDetails);

    // Perform application initialization:
    if (!gameWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

    WindowDetails propertiesWindowDetails;
    LoadString(hInstance, IDS_PROPS_WINDOW_TITLE, propertiesWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_PROPS_WINDOW, propertiesWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    propertiesWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    propertiesWindowDetails.X = 700; propertiesWindowDetails.Y = 300;
    propertiesWindowDetails.Width = 300; propertiesWindowDetails.Height = 300;
    propertiesWindowDetails.Parent = mainWindow;
    propertiesWindowDetails.ClassDetails.Icon = IDI_HAMMER;
    propertiesWindowDetails.ClassDetails.SmallIcon = IDI_HAMMER;
    propertiesWindowDetails.ClassDetails.WndProcCallback = PropertiesWindowWndProc;
    propertiesWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_3DFACE);
    Window* propertiesWindow = Window::CreateWin32Window(propertiesWindowDetails);

    // Perform application initialization:
    if (!propertiesWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }
    
    g_Engine = Engine::CreateEngine(*gameWindow);

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

LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
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

LRESULT CALLBACK PropertiesWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
