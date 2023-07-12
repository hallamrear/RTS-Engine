// Editor.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Editor.h"

#define MAX_LOADSTRING 100

static void CheckWin32Error()
{
    wchar_t s[255];
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&s, 0, NULL);
    MessageBox(NULL, s, L"Error", MB_ICONEXCLAMATION | MB_OK);
}

struct WINDOW_CLASS_DETAILS
{
    WCHAR ClassName[MAX_LOADSTRING];
    WNDPROC WndProcCallback;
    int Icon = IDI_EDITOR;
    int SmallIcon = IDI_SMALL;
    HBRUSH BackgroundColour = GetSysColorBrush(0);
    int AdditionalClassStyles;
};

struct WINDOW_DETAILS
{
    WINDOW_CLASS_DETAILS ClassDetails;

    WCHAR Title[MAX_LOADSTRING];
    DWORD WindowStyles;
    int X = CW_USEDEFAULT; int Y = CW_USEDEFAULT;
    int W = CW_USEDEFAULT; int H = CW_USEDEFAULT;
    HWND Parent = NULL;
    LPVOID lpParam = nullptr;
    int CmdShow = 1;
};

HINSTANCE applicationInstance;

// Forward declarations of functions included in this code module:
ATOM                RegisterWin32Class(HINSTANCE, const WINDOW_CLASS_DETAILS&);
BOOL                InitInstance(HWND&, const WINDOW_DETAILS&);
HWND                CreateWin32Window(HINSTANCE, const WINDOW_DETAILS&);
LRESULT CALLBACK    MainWindowWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWindowWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Store instance handle in our global variable
    applicationInstance = hInstance;

    WINDOW_DETAILS mainWindowDetails;
    LoadStringW(applicationInstance, IDS_APP_TITLE, mainWindowDetails.Title, MAX_LOADSTRING);
    LoadStringW(applicationInstance, IDC_EDITOR, mainWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    mainWindowDetails.WindowStyles = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
    mainWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_APPWORKSPACE);
    mainWindowDetails.ClassDetails.WndProcCallback = MainWindowWndProc;
    mainWindowDetails.ClassDetails.Icon = IDI_ICON1;
    mainWindowDetails.ClassDetails.SmallIcon = IDI_ICON1;
    HWND mainWindow = CreateWin32Window(applicationInstance, mainWindowDetails);

    // Perform application initialization:
    if (!mainWindow)
    {
        CheckWin32Error();
        return FALSE;
    }

    WINDOW_DETAILS childWindowDetails;
    LoadStringW(applicationInstance, IDS_CHILDONEWINDOWTITLE, childWindowDetails.Title, MAX_LOADSTRING);
    LoadStringW(applicationInstance, IDC_CHILDONECLASSNAME, childWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    childWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_TILEDWINDOW;
    childWindowDetails.X = 100; childWindowDetails.Y = 100;
    childWindowDetails.W = 300; childWindowDetails.H = 300;
    childWindowDetails.Parent = mainWindow;
    childWindowDetails.ClassDetails.WndProcCallback = ChildWindowWndProc;
    childWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_GRADIENTACTIVECAPTION);

    HWND childWindow = CreateWin32Window(applicationInstance, childWindowDetails);

    // Perform application initialization:
    if (!childWindow)
    {
        CheckWin32Error();
        return FALSE;
    }

    WINDOW_DETAILS childTwoWindowDetails;
    LoadStringW(applicationInstance, IDS_CHILDTWOWINDOWTITLE, childTwoWindowDetails.Title, MAX_LOADSTRING);
    LoadStringW(applicationInstance, IDC_CHILDTWOCLASSNAME, childTwoWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    childTwoWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_TILEDWINDOW;
    childTwoWindowDetails.X = 700; childTwoWindowDetails.Y = 300;
    childTwoWindowDetails.W = 300; childTwoWindowDetails.H = 300;
    childTwoWindowDetails.Parent = mainWindow;
    childTwoWindowDetails.ClassDetails.WndProcCallback = ChildWindowWndProc;
    childTwoWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_GRADIENTACTIVECAPTION);
    HWND childTwoWindow = CreateWin32Window(applicationInstance, childTwoWindowDetails);

    // Perform application initialization:
    if (!childTwoWindow)
    {
        CheckWin32Error();
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(applicationInstance, MAKEINTRESOURCE(IDC_EDITOR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

HWND CreateWin32Window(HINSTANCE hInstance, const WINDOW_DETAILS& details)
{
    ATOM classAtom = RegisterWin32Class(hInstance, details.ClassDetails);

    if (!classAtom)
    {
        CheckWin32Error();
        return 0;
    }

    HWND windowHandle;

    // Perform application initialization:
    if (!InitInstance(windowHandle, details))
    {
        CheckWin32Error();
        return 0;
    }

    return windowHandle;
}

//  FUNCTION: RegisterWin32Class()
// 
//  PURPOSE: Registers the window class.
//
ATOM RegisterWin32Class(HINSTANCE hInstance, const WINDOW_CLASS_DETAILS& classDetails)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | classDetails.AdditionalClassStyles;
    wcex.lpfnWndProc = classDetails.WndProcCallback;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(classDetails.Icon));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = classDetails.BackgroundColour;
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EDITOR);
    wcex.lpszClassName = classDetails.ClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(classDetails.SmallIcon));

    ATOM registeredClass = RegisterClassExW(&wcex);

    if (!registeredClass)
    {
        CheckWin32Error();
        return 0;
    }

    return registeredClass;
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//   In this function, we save the instance handle in a global variable and
//   create and display the main program window.
//
BOOL InitInstance(HWND& outWnd, const WINDOW_DETAILS& details)
{
    HWND hWnd = CreateWindow(
        details.ClassDetails.ClassName,
        details.Title,
        details.WindowStyles,
        details.X, details.Y,
        details.W, details.H,
        details.Parent,
        nullptr,
        applicationInstance,
        details.lpParam);

    if (!hWnd)
    {
        CheckWin32Error();
        return FALSE;
    }

    ShowWindow(hWnd, details.CmdShow);
    UpdateWindow(hWnd);

    outWnd = hWnd;

    return TRUE;
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
                DialogBox(applicationInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            DialogBox(applicationInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
