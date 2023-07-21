#include "pch.h"
#include "RenderWindow.h"
#include "Editor.h"
#include <Engine.h>
#include <CommCtrl.h>

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

    if (!renderWindow)
    {
        return nullptr;
    }

    Window* statusBar = nullptr;
    WindowDetails renderWindowBarDetails;
    strcpy_s(renderWindowBarDetails.ClassDetails.ClassName, STATUSCLASSNAME);
    renderWindowBarDetails.WindowStyles = SBT_TOOLTIPS | SBARS_TOOLTIPS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE;
    renderWindowBarDetails.Width = 0; renderWindowBarDetails.Height = 0; renderWindowBarDetails.X = 0; renderWindowBarDetails.Y = 0;
    renderWindowBarDetails.Parent = renderWindow;
    renderWindowBarDetails.ClassDetails.WndProcCallback = RenderWindowExtraDetailsWndProc;
    renderWindowBarDetails.ClassDetails.AdditionalClassStyles = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
    statusBar = Window::CreateWin32Window(renderWindowBarDetails);

    //Split bar into 4 parts.
    int parts[] = { 500, 600, 700, -1 };

    SendMessage(statusBar->GetWindowHandle(), SB_SETPARTS, 4, (LPARAM)parts);
    SendMessage(statusBar->GetWindowHandle(), SB_SETTEXT, 0, (LPARAM)"Status Bar");
    SendMessage(statusBar->GetWindowHandle(), SB_SETTEXT, 1, (LPARAM)"Cells");
    SendMessage(statusBar->GetWindowHandle(), SB_SETTEXT, 2, (LPARAM)"text");
    SendMessage(statusBar->GetWindowHandle(), SB_SETTEXT, 3, (LPARAM)"text 2");

    return renderWindow;
}

LRESULT CALLBACK RenderWindowExtraDetailsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //case WM_SIZE:
        //    if(statusBar)
        //        SendMessage(statusBar->GetWindowHandle(), WM_SIZE, 0, 0);
        //    break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
