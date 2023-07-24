#include "pch.h"
#include "RenderWindow.h"
#include "Editor.h"
#include <Engine.h>

HWND statusBar = NULL;
Window* myStatusBar = nullptr;

Window* CreateRenderWindow(HINSTANCE hInstance, Window* parentWindow)
{
    WindowDetails renderWindowDetails;
    LoadString(hInstance, IDS_GAME_WINDOW_TITLE, renderWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_GAME_WINDOW, renderWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    renderWindowDetails.WindowStyles = WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    renderWindowDetails.X = 100; renderWindowDetails.Y = 100;
    renderWindowDetails.Width = 800; renderWindowDetails.Height = 600;
    renderWindowDetails.Parent = parentWindow;
    renderWindowDetails.ClassDetails.Icon = IDI_PICKAXE;
    renderWindowDetails.ClassDetails.SmallIcon = IDI_PICKAXE;
    renderWindowDetails.ClassDetails.WndProcCallback = Engine::WindowsCallbackProcedure;
    renderWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_GRADIENTACTIVECAPTION);
    Window* renderWindow = Window::CreateWin32Window(renderWindowDetails);

    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;
    if (!InitCommonControlsEx(&iccx))
        return FALSE;

    WindowDetails statusBarDetails;
    std::string className = STATUSCLASSNAME;
    strcpy_s(statusBarDetails.ClassDetails.ClassName, STATUSCLASSNAME);
    statusBarDetails.WindowStyles = SBARS_SIZEGRIP | CCS_BOTTOM | CCS_NOMOVEY | CCS_TOP | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE;
    statusBarDetails.Parent = renderWindow;
    statusBarDetails.ClassDetails.MenuName = IDC_GAME_WINDOW_EXTRAS;
    statusBarDetails.ClassDetails.AdditionalClassStyles = WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
    myStatusBar = Window::CreateWin32Window(statusBarDetails);
    //Split bar into 4 parts.
    int parts[4] = { 200, 300, 400, -1 };
    SendMessage(myStatusBar->GetWindowHandle(), SB_SETPARTS, (WPARAM)4, (LPARAM)parts);
    SendMessage(myStatusBar->GetWindowHandle(), SB_SETTEXT, 0, (LPARAM)"Part 1");
    SendMessage(myStatusBar->GetWindowHandle(), SB_SETTEXT, 1, (LPARAM)"Part 2");
    SendMessage(myStatusBar->GetWindowHandle(), SB_SETTEXT, 2, (LPARAM)"Part 3");
    SendMessage(myStatusBar->GetWindowHandle(), SB_SETTEXT, 3, (LPARAM)"Part 4");
    SendMessage(myStatusBar->GetWindowHandle(), WM_SIZE, 0, 0);

    return renderWindow;
}

LRESULT CALLBACK RenderWindowExtraDetailsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        RECT rc = { 0, 0, 0, 0 };
        GetClientRect(NULL, &rc);

        int nHalf = 200;
        int nParts[] = { nHalf, nHalf + nHalf / 3, nHalf + nHalf * 2 / 3, -1 };

        if (statusBar == NULL)
            return 0;
        else
            SendMessage(statusBar, SB_SETPARTS, 4, (LPARAM)&nParts);
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}