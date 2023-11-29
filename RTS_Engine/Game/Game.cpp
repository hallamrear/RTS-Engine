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
    
    /* 
    for (int j = 0; j < 2500; j++)
    {
        int x = (rand() % 1000) - 500;
        int y = (rand() % 1000) - 500;
        int z = (rand() % 1000) - 500;
        int roll = (rand() % 359);
        int pitch = (rand() % 359);
        int yaw = (rand() % 359);
        int s = rand() % 20;

        Bennett::Entity* entity = GetWorld().SpawnEntity(std::to_string(j), glm::vec3(s), glm::vec3(x, y, z), glm::vec3(roll, pitch, yaw));
        entity->SetModel(am.GetModel("1x1_Cube"));
        entity->AddBroadPhaseCollider(Bennett::ColliderType::Sphere, glm::vec3(rand() % 100));
    }
    */
    Bennett::Entity* entity = nullptr;

    //entity = GetWorld().SpawnEntity("Floor", glm::vec3(100.0f, 0.5f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f));
    //entity->SetModel(am.GetModel("1x1_Cube"));
    //entity->SetTexture(am.GetTexture("Required/Debug"));
    //entity->AddBroadPhaseCollider(Bennett::ColliderType::AABB, glm::vec3(1.0f));

    entity = GetWorld().SpawnEntity("test", glm::vec3(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f));
    entity->SetModel(am.GetModel("1x1_Cube"));
    //entity->AddBroadPhaseCollider(Bennett::ColliderType::Sphere, glm::vec3(1.0f));

    entity = GetWorld().SpawnTestEntity("testMoveable");
    entity->SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));
    entity->SetModel(am.GetModel("1x1_Cube"));
    entity->SetTexture(am.GetTexture("Floor"));
    //entity->AddBroadPhaseCollider(Bennett::ColliderType::Sphere, glm::vec3(1.0f));

    /*for (int i = -10; i < 10; i++)
    {
        for (int j = -10; j < 10; j++)
        {
            std::string name = std::to_string(i) + "_" + std::to_string(j);
            entity = GetWorld().SpawnEntity(name);
            entity->SetPosition(glm::vec3(i, 0.0f, j));
            entity->SetModel(am.GetModel("1x1_Cube"));
        }
    }*/

  /*  for (size_t i = 1; i < 3; i++)
    {
        Bennett::Entity* ent = GetWorld().SpawnEntity(std::to_string(i), glm::vec3(1.0f), glm::vec3(8.0f * i, 0.0f, 0.0f), glm::vec3(0.0f));
        ent->SetModel(am.GetModel("1x1_Cube"));
        ent->AddBroadPhaseCollider((Bennett::ColliderType)(i), glm::vec3(5.0f, 5.0f, 5.0f));
    }*/

    if (!entity)
        return false;

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    return true;
}

void Game::RunGameLoop()
{
    auto lTime = std::chrono::steady_clock::now();
    auto cTime = lTime;
    std::chrono::duration<float> clockDelta = { };
    float dTime = 0.0f;

    Bennett::Entity* entityOne = GetWorld().GetEntity("test");
    Bennett::Entity* entityTwo = GetWorld().GetEntity("testMoveable");
    //Bennett::Entity* entityFloor = GetWorld().GetEntity("floor");

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

        const glm::vec3& pos = entityOne->GetPosition();
        const Bennett::SphereCollider& collOne = (const Bennett::SphereCollider&)*entityOne->GetCollider();
        const Bennett::SphereCollider& collTwo = (const Bennett::SphereCollider&)*entityTwo->GetCollider();
        const Bennett::Ray ray = Bennett::Ray(GetCameraController().GetCurrentCamera().GetPosition(), GetCameraController().GetCurrentCamera().GetForwardVector());

     /*   for (size_t i = 0; i < ; i++)
        {

            if ()
        }*/

        //bool result = Bennett::Collision::CheckCollision<Bennett::SphereCollider, Bennett::SphereCollider>(collOne, collTwo);
        //bool result = Bennett::Collision::CheckCollision<Bennett::Ray, Bennett::AABBCollider>(ray, terrainCollider);
        //Log("Collision: " + std::to_string(result), LOG_SAFE);

        Update(dTime);
        Render();

        lTime = cTime;
    }
}
