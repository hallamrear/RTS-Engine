#include "pch.h"
#include "ToolWindow.h"
#include "Editor.h"

Window* CreateToolWindow(HINSTANCE hInstance, Window* parentWindow)
{
    WindowDetails ToolWindowDetails;
    LoadString(hInstance, IDS_TOOL_WINDOW_TITLE, ToolWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TOOL_WINDOW, ToolWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    ToolWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    ToolWindowDetails.X = 700; ToolWindowDetails.Y = 300;
    ToolWindowDetails.Width = 300; ToolWindowDetails.Height = 300;
    ToolWindowDetails.Parent = parentWindow;
    ToolWindowDetails.ClassDetails.Icon = IDI_HAMMER;
    ToolWindowDetails.ClassDetails.SmallIcon = IDI_HAMMER;
    ToolWindowDetails.ClassDetails.WndProcCallback = ToolWindowWndProc;
    ToolWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_3DFACE);
    return Window::Create(ToolWindowDetails);
}

LRESULT CALLBACK ToolWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
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