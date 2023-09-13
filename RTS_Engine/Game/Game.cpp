#include "pch.h"
#include "Game.h"
#include <chrono>
#include <Window.h>
#include <InputMonitor.h>
#include <ServiceLocator.h>
#include <AssetManager.h>
#include <Vertex.h>

Game::Game()
{
    m_Window = nullptr;
}

Game::~Game()
{
    if (m_Window)
    {
        Bennett::Window::Destroy(m_Window);
    }
}

bool Game::Initialise()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    Bennett::WindowDetails mainWindowDetails;
    LoadString(hInstance, IDS_APP_TITLE, mainWindowDetails.Title, MAX_LOADSTRING);
    LoadString(hInstance, IDC_GAME, mainWindowDetails.ClassDetails.ClassName, MAX_LOADSTRING);
    mainWindowDetails.WindowStyles = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
    mainWindowDetails.ClassDetails.BackgroundColour = GetSysColorBrush(COLOR_APPWORKSPACE);
    mainWindowDetails.ClassDetails.WndProcCallback = Engine::WindowsCallbackProcedure;
    mainWindowDetails.ClassDetails.Icon = IDI_GAME;
    mainWindowDetails.ClassDetails.SmallIcon = IDI_SMALL;
    m_Window = Bennett::Window::Create(mainWindowDetails);

    // Perform application initialization:
    if (!m_Window)
    {
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

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

    m_EngineControls = new Bennett::InputMonitor(keys);

    if (!InitialiseEngineSystems(*m_Window))
    {
        Log("Failed to initialise engine systems.", LOG_CRITICAL);
        return FALSE;
    }

    Bennett::AssetManager& am = Bennett::ServiceLocator::GetAssetManager();

    for (int j = 0; j < 20; j += 1)
    {
        Bennett::Entity* entity = GetWorld().SpawnEntity(std::to_string(c));
        entity->SetModel(am.GetModel("1x1_Quad"));
        int x = (rand() % 30) - 10;
        int y = (rand() % 30) - 10;
        int z = (rand() % 30) - 10;
        entity->SetPosition(glm::vec3(x, y, z));
        int r = rand() % 10;
        entity->SetScale(glm::vec3(r));
    }

    m_CameraController.SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    m_CameraController.GetCurrentCamera().SetPosition(glm::vec3(-15.0f, 15.0f, -15.0f));

    return true;
}

void Game::RunGameLoop()
{
    auto lTime = std::chrono::steady_clock::now();
    auto cTime = lTime;
    float dTime = 0.0f;

    MSG msg{};
    while (IsRunning())
    {
        while(PeekMessage(&msg, m_Window->GetWindowHandle(), 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        cTime = std::chrono::steady_clock::now();
        dTime = (cTime - lTime).count();

        if (dTime > TIMESTEP_CAP)
            dTime = TIMESTEP_CAP;

        ProcessInput(dTime);
        Update(dTime);
        Render();

        lTime = cTime;
    }
}
