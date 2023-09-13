#pragma once
#include "pch.h"
#include "Editor.h"
#include "resource.h"

#include <BennettDefines.h>
#include <chrono>

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
        AllocConsole();

    FILE* file = nullptr;
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
#endif
    INITCOMMONCONTROLSEX controls{};
    controls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    controls.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&controls);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITOR));

    Editor* m_Editor = new Editor();

    if (m_Editor->Initialise())
    {
        m_Editor->RunGameLoop();
    }

    Log("Engine has finished running, it is now closing.", LOG_SAFE);
    delete m_Editor;
    m_Editor = nullptr;

    return 0;
}