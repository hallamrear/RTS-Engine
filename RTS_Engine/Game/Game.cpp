#include "pch.h"
#include "Game.h"
#include <chrono>
#include <Collision/Collider/AABBCollider.h>
#include <Collision/Collider/Ray.h>
#include <Collision/Collider/SphereCollider.h>
#include <Collision/Collision.h>
#include <Rendering/Vertex.h>
#include <Rendering/Window.h>
#include <System/InputMonitor.h>
#include <System/Manager/AssetManager.h>
#include <System/ServiceLocator.h>
#include <World/Entity.h>

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

Bennett::Entity* origin = nullptr;
Bennett::Entity* terrain = nullptr;

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

    if (!InitialiseEngineSystems(*m_Window))
    {
        Log("Failed to initialise engine systems.", LOG_CRITICAL);
        return FALSE;
    }

    Bennett::AssetManager& am = Bennett::ServiceLocator::GetAssetManager();
    
    terrain = GetWorld().CreateTerrain();

 /*   origin = GetWorld().SpawnEntity("TerrainOrigin");
    origin->SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));
    origin->SetModel(am.GetModel("1x1_Cube"));
    origin->GetModel()->SetTexture(am.GetTexture("Car4"));*/

    Bennett::Entity* entity = nullptr;
    Bennett::Terrain* t = (Bennett::Terrain*)terrain;

    for (size_t i = 0; i < TERRAIN_CHUNK_COUNT; i++)
    {
        entity = GetWorld().SpawnEntity("Origin_" + std::to_string(i));
        entity->SetPosition(glm::vec3(t->m_ChunkLocations[i].x, 0.0f, t->m_ChunkLocations[i].y));
        entity->SetModel(am.GetModel("1x1_Cube"));
        entity->GetModel()->SetTexture(am.GetTexture("Car4"));
    }

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    return true;
}

void Game::RunGameLoop()
{
    auto lTime = std::chrono::steady_clock::now();
    auto cTime = lTime;
    std::chrono::duration<float> clockDelta = { };
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
        clockDelta = std::chrono::duration_cast<std::chrono::milliseconds>(cTime - lTime);
        dTime = clockDelta.count();

        if (dTime > TIMESTEP_CAP)
            dTime = TIMESTEP_CAP;

        ProcessInput(dTime);
        Update(dTime);

        float rotSpeed = 50.0f;
        float r = 10.0f;    
        float t = glm::radians(90.0f);
        static float s = 0.0f;
        s += dTime * rotSpeed;
        s = fmod(s, 360.0f);

        //glm::vec3 position{};
        //position.x = r * cos(glm::radians(s)) * sin(t);
        //position.y = 0.0f;
        //position.z = r * sin(glm::radians(s)) * sin(t);
        //terrain->SetPosition(position);
        //origin->SetPosition(terrain->GetPosition());

        Render();

        lTime = cTime;
    }
}
