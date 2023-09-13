#include "pch.h"
#include "PropertiesWindow.h"
#include "Editor.h"

Window* CreatePropertiesWindow(HINSTANCE hInstance, Window* parentWindow)
{
    WindowDetails propertiesWindowDetails;
    LoadString(hInstance, IDS_PROPS_WINDOW_TITLE, propertiesWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_PROPS_WINDOW, propertiesWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    propertiesWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    propertiesWindowDetails.X = 700; propertiesWindowDetails.Y = 300;
    propertiesWindowDetails.Width = 300; propertiesWindowDetails.Height = 300;
    propertiesWindowDetails.Parent = parentWindow;
    propertiesWindowDetails.ClassDetails.Icon = IDI_HAMMER;
    propertiesWindowDetails.ClassDetails.SmallIcon = IDI_HAMMER;
    propertiesWindowDetails.ClassDetails.WndProcCallback = PropertiesWindowWndProc;
    propertiesWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_3DFACE);
    return Window::Create(propertiesWindowDetails);
}

LRESULT CALLBACK PropertiesWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        //Minimise a tool window when hitting the X button.
        PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}