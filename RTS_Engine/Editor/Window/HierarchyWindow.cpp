#include "pch.h"
#include "HierarchyWindow.h"
#include "Editor.h"

Window* CreateHierarchyWindow(HINSTANCE hInstance, Window* parentWindow)
{
    WindowDetails HierarchyWindowDetails;
    LoadString(hInstance, IDS_HIERARCHY_WINDOW_TITLE, HierarchyWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_HIERARCHY_WINDOW, HierarchyWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    HierarchyWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    HierarchyWindowDetails.X = 700; HierarchyWindowDetails.Y = 300;
    HierarchyWindowDetails.Width = 300; HierarchyWindowDetails.Height = 300;
    HierarchyWindowDetails.Parent = parentWindow;
    HierarchyWindowDetails.ClassDetails.Icon = IDI_HAMMER;
    HierarchyWindowDetails.ClassDetails.SmallIcon = IDI_HAMMER;
    HierarchyWindowDetails.ClassDetails.WndProcCallback = HierarchyWindowWndProc;
    HierarchyWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_3DFACE);
    return Window::Create(HierarchyWindowDetails);
}

LRESULT CALLBACK HierarchyWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_CLOSE:
        //Minimise a Hierarchy window when hitting the X button.
        PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}