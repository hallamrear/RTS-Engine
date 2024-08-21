#include "pch.h"
#include "Game.h"
#include <chrono>
#include <array>
#include <Physics/Collision/Collider/AABBCollider.h>
#include <Physics/Collision/Collider/Ray.h>
#include <Physics/Collision/Collider/SphereCollider.h>
#include <Physics/Collision/CollisionDetection.h>
#include <Rendering/Vertex.h>
#include <Rendering/Window.h>
#include <System/InputMonitor.h>
#include <System/Assets/AssetManager.h>
#include <System/ServiceLocator.h>
#include <System/Transform.h>
#include <World/WorldChunk.h>
#include <World/Entity/BEntity.h>
#include <World/Actor/BActor.h>
#include <Defines/HelperFunctions.h>

using namespace Bennett;

Game::Game()
{
    m_Window = nullptr;
    m_SelectedEntities = std::vector<BActor*>();
}

Game::~Game()
{
    m_SelectedEntities.clear();

    if (m_Window)
    {
        Window::Destroy(m_Window);
    }
}

BActor* car = nullptr;
BActor* ground = nullptr;
InputMonitor* inputMonitor = nullptr;

std::vector<std::pair<glm::vec3, glm::vec3>> lines;

bool Game::Initialise()
{
    srand(time(NULL));

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
        Log(LOG_SERIOUS, GetLastWin32Error().c_str());
        return FALSE;
    }

    if (!InitialiseEngineSystems(*m_Window))
    {
        Log(LOG_CRITICAL, "Failed to initialise engine systems.\n");
        return FALSE;
    }

    AssetManager& am = ServiceLocator::GetAssetManager();

    lines = std::vector<std::pair<glm::vec3, glm::vec3>>();

    std::vector<int> keys =
    {
        BENNETT_MOUSE_LEFT, //Draw cam Line
        BENNETT_MOUSE_RIGHT,
        BENNETT_KEY_C,
        BENNETT_KEY_LSHIFT
    };

    inputMonitor = new InputMonitor(keys);

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::STANDARD_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    glm::vec3 pos;
    
    for (size_t i = 0; i <= 1; i++)
    {
        std::string name = std::to_string(i);
        pos = glm::vec3(rand() % 30 - 15, 0.0f, rand() % 30 - 15);
        GetWorld().SpawnActor(name, Transform(glm::vec3(1.0f), pos, glm::vec3(0.0f)));
    }

    pos = glm::vec3(25.0f, 0.0f, 0.0f);
    GetWorld().SpawnActor("TestUnit", Transform(glm::vec3(1.0f), pos, glm::vec3(0.0f)));
  
    BProp& testBuilding = *GetWorld().SpawnProp("TestBuilding", Transform());
    testBuilding.SetModel(am.GetModel("Buildings/TestBuilding.gltf"));
    testBuilding.SetTexture(am.GetTexture("Buildings/TestBuilding"));
    testBuilding.GeneratePhysicsColliderFromModel(ColliderType::OBB);

    BProp& floor = *GetWorld().SpawnProp("Floor", Transform(glm::vec3(30.0f, 5.0f, 30.0f), glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(0.0f)));
    floor.SetModel(am.GetModel("1x1_Cube"));
    floor.SetTexture(am.GetTexture("Terrain"));
    floor.GeneratePhysicsColliderFromModel(ColliderType::AABB);

    std::vector<glm::ivec2> ids
    {
        glm::ivec2(-1, -1), glm::ivec2(+0, -1), glm::ivec2(+1, -1),
        glm::ivec2(-1, +0), glm::ivec2(+0, +0), glm::ivec2(+1, +0),
        glm::ivec2(-1, +1), glm::ivec2(+0, +1), glm::ivec2(+1, +1),
    };

    GetWorld().PreloadChunks(ids);

    SetEngineState(ENGINE_STATE::RUNNING);

    return true;
}

bool pressedLast = false;

void Game::RunGameLoop()
{
    AssetManager& am = ServiceLocator::GetAssetManager();

    auto lTime = std::chrono::steady_clock::now();
    auto cTime = lTime;
    std::chrono::duration<double> clockDelta = { };
    float dTime = 0.0f;

    CollisionDetails details;

    MSG msg{};
    while (GetEngineState() == ENGINE_STATE::RUNNING)
    {
        while (PeekMessage(&msg, m_Window->GetWindowHandle(), 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        cTime = std::chrono::steady_clock::now();
        clockDelta = std::chrono::duration_cast<std::chrono::milliseconds>(cTime - lTime);
        dTime = clockDelta.count();

        if (dTime > TIMESTEP_CAP)
        {
            Log(LOG_STATUS::LOG_MINIMAL, "Capped timestep to %f\n", TIMESTEP_CAP);
            dTime = TIMESTEP_CAP;
        }

        std::string dtStr = std::to_string(dTime);
        m_Window->SetTitle(dtStr.c_str());

        ProcessInput(dTime);
        Update(dTime);
        Render();

        float rotSpeed = 10.0f;
        float r = 10.0f;
        float t = glm::radians<float>(90.0f);
        static float s = 0.0f;
        s += dTime * rotSpeed;
        s = fmod(s, 360.0f);

        Ray ndcRay = GetCameraController().GetCurrentCamera().Raycast(inputMonitor->GetMousePositionNDC());

        if (inputMonitor->GetKeyState(BENNETT_MOUSE_LEFT))
        {
            float t = 0.0f;
            if (Collision::RayPlaneIntersection(glm::vec3(0.0f), glm::normalize(BENNETT_UP_VECTOR), ndcRay, t))
            {
                glm::vec3 position = GetCameraController().GetCurrentCamera().GetPosition() + (ndcRay.GetDirection() * t);

                WorldChunk* chunk = GetWorld().GetWorldChunk(position);
                if (chunk != nullptr)
                {
                    std::vector<BEntity*>& entities = chunk->GetAllEntities();
                    for (auto itr : entities)
                    {
                        BActor* actor = GetWorld().GetActor(itr->GetName());

                        if (actor)
                        {
                            Transform transform(glm::vec3(1.0f), position, glm::vec3(0.0f));
                            SphereCollider sphere(transform);

                            if (Collision::CheckCollision(actor->GetSelectionCollider(), sphere))
                            {
                                if (inputMonitor->GetKeyState(BENNETT_KEY_LSHIFT) == false)
                                {
                                    for (size_t i = 0; i < m_SelectedEntities.size(); i++)
                                    {
                                        m_SelectedEntities[i]->SetIsSelected(false);
                                    }
                                    m_SelectedEntities.clear();
                                }

                                actor->SetIsSelected(true);
                                m_SelectedEntities.push_back(actor);
                            }
                        }
                    }
                }
            }
        }

        if (inputMonitor->GetKeyState(BENNETT_KEY_C))
        {
            for (size_t i = 0; i < m_SelectedEntities.size(); i++)
            {
                m_SelectedEntities[i]->SetIsSelected(false);
            }

            m_SelectedEntities.clear();
            GetWorld().GetAllActors(m_SelectedEntities);

            for (size_t i = 0; i < m_SelectedEntities.size(); i++)
            {
                m_SelectedEntities[i]->SetIsSelected(true);
            }
        }

        if (inputMonitor->GetKeyState(BENNETT_MOUSE_RIGHT))
        {
            float t = 0.0f;
            if (Collision::RayPlaneIntersection(glm::vec3(0.0f), glm::normalize(BENNETT_UP_VECTOR), ndcRay, t))
            {
                glm::vec3 position = GetCameraController().GetCurrentCamera().GetPosition() + (ndcRay.GetDirection() * t);

                for (size_t i = 0; i < m_SelectedEntities.size(); i++)
                {
                    m_SelectedEntities[i]->SetTargetPosition(position);
                }
            }
        }

        if (Collision::RayPlaneIntersection(glm::vec3(0.0f), glm::normalize(BENNETT_UP_VECTOR), ndcRay, t))
        {
            glm::vec3 position = GetCameraController().GetCurrentCamera().GetPosition() + (ndcRay.GetDirection() * t);
            ServiceLocator::GetRenderer().DrawDebugCircle(position, 1.0f);
        }
        
        ServiceLocator::GetRenderer().DrawDebugLine(glm::vec3(0.0f), glm::vec3(5.0f, 0.0f, 0.0f));
        ServiceLocator::GetRenderer().DrawDebugLine(glm::vec3(0.0f), glm::vec3(0.0f, 5.0f, 0.0f));
        ServiceLocator::GetRenderer().DrawDebugLine(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 5.0f));

        for (size_t i = 0; i < lines.size(); i++)
        {
            ServiceLocator::GetRenderer().DrawDebugLine(
                lines[i].first,
                lines[i].second);
        }

        lTime = cTime;
    }
}
