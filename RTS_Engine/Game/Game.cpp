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

Bennett::Entity* entity = nullptr;
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

    //terrain = GetWorld().CreateTerrain();
    //Bennett::Terrain* t = (Bennett::Terrain*)terrain;
    ////Number of chunks * size of each cell
    //float terrainSize = TERRAIN_WIDTH * (TERRAIN_CELL_SIZE);
    //glm::vec3 pos = terrain->GetPosition();
    //glm::vec3 corners[4] =
    //{
    //    pos,
    //    glm::vec3(pos.x + terrainSize, pos.y, pos.z),
    //    glm::vec3(pos.x, pos.y, pos.z + terrainSize),
    //    glm::vec3(pos.x + terrainSize, pos.y, pos.z + terrainSize),
    //};

    entity = GetWorld().SpawnEntity("Glitch");
    entity->SetModel(am.GetModel("glitch.gltf"));
    entity->GetModel()->SetTexture(am.GetTexture("glitch"));
    entity->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));

    entity = GetWorld().SpawnEntity("shakedown");
    entity->SetRotationEuler(glm::vec3(0.0f));
    entity->SetModel(am.GetModel("shakedown.gltf"));
    entity->GetModel()->SetTexture(am.GetTexture("shakedown"));

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
    std::chrono::duration<double> clockDelta = { };
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
        {
            printf("capped timestep\n");
            dTime = TIMESTEP_CAP;
        }

        ProcessInput(dTime);
        Update(dTime);

        float rotSpeed = 10.0f;
        float r = 10.0f;    
        float t = glm::radians(90.0f);
        static float s = 0.0f;
        s += dTime * rotSpeed;
        s = fmod(s, 360.0f);

        entity->SetRotationEuler(glm::vec3(0.0f, s, 0.0f));

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
