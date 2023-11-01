// Editor.cpp : Defines the entry point for the application.

#include "pch.h"
#include <Rendering/Window.h>
#include <System/Engine.h>
#include <System/Logger.h>
#include <System/InputMonitor.h>
#include <chrono>
#include "Editor.h"
#include <Window/PropertiesWindow.h>
#include <Window/ToolWindow.h>
#include <Window/HierarchyWindow.h>
#include <Window/RenderWindow.h>

using namespace Bennett;

Editor::Editor()
{
    m_MainWindow = nullptr;
    m_EngineControls = nullptr;
    m_HierarchyWindow = nullptr;
    m_ToolWindow = nullptr;
    m_PropertiesWindow = nullptr;
    m_RenderWindow = nullptr;
}

Editor::~Editor()
{
    Window::Destroy(m_HierarchyWindow);
    Window::Destroy(m_ToolWindow);
    Window::Destroy(m_PropertiesWindow);
    Window::Destroy(m_RenderWindow);
    Window::Destroy(m_MainWindow);
}

bool Editor::Initialise()
{
    CreateWindows();

    std::vector<int> keys =
    {
        BENNETT_KEY_F1,
        BENNETT_KEY_F2,
        BENNETT_KEY_F3,
        BENNETT_KEY_F4,
        BENNETT_KEY_F5,
        BENNETT_KEY_F6,
        BENNETT_KEY_F7,
        BENNETT_KEY_F8,
        BENNETT_KEY_F9,
        BENNETT_KEY_F10,
        BENNETT_KEY_F11,
        BENNETT_KEY_F12
    };

    m_EngineControls = new InputMonitor(keys);

    if (!InitialiseEngineSystems(*m_RenderWindow))
    {
        Log("Failed to initialise engine systems.", LOG_CRITICAL);
        return FALSE;
    }
}

bool Editor::CreateWindows()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

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
    m_MainWindow = Window::Create(mainWindowDetails);

    // Perform application initialization:
    if (!m_MainWindow)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS); 
        Log("Failed to create a window.", LOG_SERIOUS);
        return false;
    }

    m_RenderWindow = CreateRenderWindow(hInstance, m_MainWindow);
    if (!m_RenderWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); Log("Failed to create a window.", LOG_SERIOUS); return false; }

    m_PropertiesWindow = CreatePropertiesWindow(hInstance, m_MainWindow);
    if (!m_PropertiesWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); Log("Failed to create a window.", LOG_SERIOUS); return false; }

    m_ToolWindow = CreateToolWindow(hInstance, m_MainWindow);
    if (!m_ToolWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); Log("Failed to create a window.", LOG_SERIOUS); return false; }

    m_HierarchyWindow = CreateHierarchyWindow(hInstance, m_MainWindow);
    if (!m_HierarchyWindow) { Log(GetLastWin32Error(), LOG_SERIOUS); Log("Failed to create a window.", LOG_SERIOUS); return false; }

    TileWindows(m_MainWindow->GetWindowHandle(), MDITILE_HORIZONTAL | MDITILE_VERTICAL, NULL, 0, NULL);

    return true;
}

void Editor::RunGameLoop()
{
    auto lTime = std::chrono::steady_clock::now();
    auto cTime = lTime;
    float dTime = 0.0f;

    MSG msg{};
    while (IsRunning())
    {
        if (PeekMessage(&msg, m_MainWindow->GetWindowHandle(), 0, 0, PM_REMOVE))
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

            //ProcessInput(dTime);
            Update(dTime);
            Render();

            lTime = cTime;
        }
    }
}

LRESULT CALLBACK MainWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Bennett::Engine* editor = Engine::GetEngineInstance();

    switch (message)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        bool state = Engine::GetInFocus();
        if (state)
        {
            Engine::WindowsCallbackProcedure(hWnd, message, wParam, lParam);
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
            case ID_FILE_NEW:
            {
                //TODO : Implement
            }
                break;

            case ID_FILE_LOAD:
            {
                //TODO : Implement
            }
                break;

            case ID_FILE_SAVE:
            {
                //TODO : Implement
            }
                break;

            case ID_FILE_SAVEAS:
            {
                //TODO : Implement
            }
                break;

            case ID_RENDERMODE_SOLID:
            {
                //TODO : Implement
            }
                break;

            case ID_RENDERMODE_WIREFRAME:
            {
                //TODO : Implement
            }
                break;

            case IDM_EXIT:
            {
                DestroyWindow(hWnd);
            }
                break;

            case IDM_ABOUT:
                DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case ID_ARRANGEWINDOWS_4SQUARES:
                TileWindows(hWnd, MDITILE_HORIZONTAL | MDITILE_VERTICAL, NULL, 0, NULL);
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