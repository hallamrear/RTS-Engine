#include "pch.h"
#include "Game.h"
#include <chrono>
#include <array>
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
#include <System/Transform.h>

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

std::array<Bennett::Entity*, 8> cornerEntities{};
Bennett::AABBCollider* collider;
Bennett::Entity* glitch = nullptr;
Bennett::Entity* car = nullptr;
Bennett::Entity* glitchPosition = nullptr;
Bennett::Entity* carPosition = nullptr;
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



    car = GetWorld().SpawnTestEntity("shakedown");
    car->GetTransform().SetRotationEuler(glm::vec3(0.0f));
    car->SetModel(am.GetModel("shakedown.gltf"));
    car->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    car->GetModel()->SetTexture(am.GetTexture("shakedown"));
    car->GetTransform().SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    carPosition = GetWorld().SpawnEntity("car position");
    carPosition->SetModel(am.GetModel("1x1_Cube"));
    carPosition->GetTransform().SetPosition(car->GetTransform().GetPosition());

    glitch = GetWorld().SpawnTestEntity("Glitch");
    glitch->SetModel(am.GetModel("glitch.gltf"));
    glitch->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    glitch->GetModel()->SetTexture(am.GetTexture("glitch"));
    glitch->GetTransform().SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));

    glitchPosition = GetWorld().SpawnEntity("glitch position");
    glitchPosition->SetModel(am.GetModel("1x1_Cube"));
    glitchPosition->GetTransform().SetPosition(glitch->GetTransform().GetPosition());

    collider = (Bennett::AABBCollider*)car->GetCollider();

    for (size_t i = 0; i < 8; i++)
    {
        cornerEntities[i] = GetWorld().SpawnEntity(std::to_string(i));
        cornerEntities[i]->SetModel(am.GetModel("1x1_Cube"));
        cornerEntities[i]->GetTransform().SetScale(glm::vec3(0.125f));
    }

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    return true;
}

float easeInOutBack(float x)
{
    const float c1 = 1.70158;
    const float c2 = c1 * 1.525;
    return x < 0.5
        ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
        : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
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

        const auto& c = collider->GetCorners();
        for (size_t i = 0; i < 8; i++)
        {
            cornerEntities[i]->GetTransform().SetPosition(c[i]);
        }

        float rotSpeed = 10.0f;
        float r = 10.0f;    
        float t = glm::radians(90.0f);
        static float s = 0.0f;
        s += dTime * rotSpeed;
        s = fmod(s, 360.0f);

        glm::vec3 off = glm::vec3(glm::vec3((sinf(s / 5.0f) * 2.0f), 0.0f, 0.0f));
        //collider->SetOffset(off);

        if (glitch)
        {
            glitch->GetTransform().SetRotationEuler(glm::vec3(0.0f, s / 4, 0.0f));
            glitch->GetTransform().SetScale(glm::vec3((sinf(s / 2.0f) * 0.8f) + 1.0f));
            glitchPosition->GetTransform().SetPosition(glitch->GetTransform().GetPosition());
        }
        
        if (car)
        {
            carPosition->GetTransform().SetPosition(car->GetTransform().GetPosition());
            car->GetTransform().SetScale(glm::vec3((sinf(s / 2.0f) * 0.5f) + 1.0f));
            car->GetTransform().SetRotationEuler(glm::vec3(0.0f, s / 8, 0.0f));
        }

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
