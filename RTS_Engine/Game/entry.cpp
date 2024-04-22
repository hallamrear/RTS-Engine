#include "pch.h"
#include "resource.h"
#include "Game.h"
#include <System/Logger.h>
#include <Defines/BennettDefines.h>

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int argc = __argc;
    char** argv = __argv;

#ifdef _DEBUG
    if (!AttachConsole(ATTACH_PARENT_PROCESS))   // try to hijack existing console of command line
        AllocConsole();                           // or create your own.

    FILE* file = nullptr;
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
#endif

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME));

    Game* game = new Game();

    if (!game)
    {
        Log(LOG_CRITICAL, "Failed to create engine.");
        return 0;
    }

    if (!game->Initialise())
    {
        Log(LOG_CRITICAL, "Failed to create engine.");
        return 0;
    }

    game->RunGameLoop();

    Log(LOG_SAFE, "Engine has finished running, it is now closing.");
    delete game;
    game = nullptr;

    return 0;
};