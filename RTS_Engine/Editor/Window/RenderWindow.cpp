#include "pch.h"
#include "RenderWindow.h"
#include "Editor.h"
#include <System/Engine.h>

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
    Window* renderWindow = Window::Create(renderWindowDetails);
    return renderWindow;
}