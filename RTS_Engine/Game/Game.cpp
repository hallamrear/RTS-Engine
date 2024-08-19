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
}

Game::~Game()
{
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
        BENNETT_KEY_C
    };

    inputMonitor = new InputMonitor(keys);

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::STANDARD_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    //ground = GetWorld().SpawnActor("ChunkLoader");
    //ground = GetWorld().SpawnActor("HeightTester", Transform());
    //ground->SetModel(am.GetModel("1x1_Cube"));

    glm::vec3 pos;
    
    for (size_t i = 0; i <= 1; i++)
    {
        std::string name = std::to_string(i);
        pos = glm::vec3(rand() % 20 - 10, 0.0f, rand() % 20 - 10);
        GetWorld().SpawnActor(name, Transform(glm::vec3(1.0f), pos, glm::vec3(0.0f)));
        GetWorld().GetEntity(name)->SetModel(am.GetModel("1x1_Cube"));
        GetWorld().GetEntity(name)->SetTexture(am.GetTexture("green"));
        GetWorld().GetEntity(name)->AddCollider(ColliderType::Sphere, glm::vec3(5.0f, 5.0f, 5.0f));
    }

    pos = glm::vec3(rand() % 20 - 10, 0.0f, rand() % 20 - 10);
    GetWorld().SpawnActor("TestUnit", Transform(glm::vec3(1.0f), pos, glm::vec3(0.0f)));
    GetWorld().GetEntity("TestUnit")->SetModel(am.GetModel("Car3.gltf"));
    GetWorld().GetEntity("TestUnit")->SetTexture(am.GetTexture("Car3"));
    GetWorld().GetEntity("TestUnit")->AddCollider(ColliderType::Sphere, glm::vec3(5.0f, 5.0f, 5.0f));
  
    std::vector<glm::ivec2> ids
    {
        glm::ivec2(-1, -1), glm::ivec2(+0, -1), glm::ivec2(+1, -1),
        glm::ivec2(-1, +0), glm::ivec2(+0, +0), glm::ivec2(+1, +0),
        glm::ivec2(-1, +1), glm::ivec2(+0, +1), glm::ivec2(+1, +1),
    };

    GetWorld().PreloadChunks(ids);

    //ground = GetWorld().SpawnActor("Floor");
    //ground->SetModel(am.GetModel("1x1_Cube"));
    //ground->GetTransform().SetScale(glm::vec3(20.0f, 1.0f, 20.0f));
    //ground->GetTransform().Translate(glm::vec3(0.0f, -5.0f, 0.0f));
    //ground->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);

    //car = GetWorld().SpawnActor("Car", Transform());
    //car->SetModel(am.GetModel("Car3.gltf"));
    //car->GenerateColliderFromModel(Bennett::ColliderType::OBB);

    //car = GetWorld().SpawnActor("Car", Transform(glm::vec3(5.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f)));
    //car->SetModel(am.GetModel("2D_Unit_Circle"));
    //car->SetTexture(am.GetTexture("red"));

    /*
    for (size_t i = 0; i < 100; i++)
    {
        glm::vec3 position = glm::vec3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);
        ground = GetWorld().SpawnActor(std::to_string(i), Transform(glm::vec3(1.0f), position, glm::vec3(0.0f)));
        ground->SetModel(am.GetModel("1x1_Cube"));
        ground->GenerateColliderFromModel(Bennett::ColliderType::OBB);
    }
    */

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
            if (pressedLast == false)
            {
                if (inputMonitor->GetKeyState(BENNETT_MOUSE_LEFT))
                {
                    Transform transform;
                    float t = 0.0f;
                    if (Collision::RayPlaneIntersection(glm::vec3(0.0f), glm::normalize(BENNETT_UP_VECTOR), ndcRay, t))
                    {
                        glm::vec3 position = GetCameraController().GetCurrentCamera().GetPosition() + (ndcRay.GetDirection() * t);

                        std::vector<BEntity*>& entities = GetWorld().GetWorldChunk(position)->GetAllEntities();

                        if (entities.size() > 0)
                        {
                            for (auto itr : entities)
                            {
                                BActor* actor = dynamic_cast<BActor*>(GetWorld().GetEntity(itr->GetName()));

                                if (actor)
                                {
                                    if (Helper::Distance(position, actor->GetTransform().GetPosition()) < 5.0f)
                                    {
                                        actor->SetIsSelected(true);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            pressedLast = true;
        }
        else
        {
            pressedLast = false;
        }

        if (inputMonitor->GetKeyState(BENNETT_MOUSE_RIGHT))
        {
            float t = 0.0f;
            if (Collision::RayPlaneIntersection(glm::vec3(0.0f), glm::normalize(BENNETT_UP_VECTOR), ndcRay, t))
            {
                glm::vec3 position = GetCameraController().GetCurrentCamera().GetPosition() + (ndcRay.GetDirection() * t);
                GetWorld().GetEntity("TestUnit")->SetTargetPosition(position);
            }
        }

        if (Collision::RayPlaneIntersection(glm::vec3(0.0f), glm::normalize(BENNETT_UP_VECTOR), ndcRay, t))
        {
            glm::vec3 position = GetCameraController().GetCurrentCamera().GetPosition() + (ndcRay.GetDirection() * t);
            ServiceLocator::GetRenderer().DrawDebugCircle(position, 1.0f);
        }
 
        //if (ground->GetCollider() != nullptr && car->GetCollider() != nullptr)
        //{
        //    details.Depth = 0.0f;
        //    details.Normal = glm::vec3(0.0f);

        //    if (Collision::CheckCollision(*ground->GetCollider(), *car->GetCollider(), &details))
        //    {
        //        check->GetTransform().SetPosition(glitch->GetTransform().GetPosition() + (details.Normal * details.Depth));

        //        ServiceLocator::GetRenderer().DrawDebugLine(
        //            details.CollisionPoints[0].Subject->GetTransform().GetPosition() + details.CollisionPoints[0].Subject->GetOffset(),
        //            details.CollisionPoints[0].HitPoint);
        //        check->GetTransform().SetPosition(details.CollisionPoints[0].HitPoint);

        //        ServiceLocator::GetRenderer().DrawDebugLine(
        //            details.CollisionPoints[1].Subject->GetTransform().GetPosition() + details.CollisionPoints[1].Subject->GetOffset(),
        //            details.CollisionPoints[1].HitPoint);
        //        check_two->GetTransform().SetPosition(details.CollisionPoints[1].HitPoint);

        //        Transform& transformA = glitch->GetTransform();
        //        Transform& transformB = car->GetTransform();

        //        //if (glitch->GetRigidbody()->IsStatic() == true && car->GetRigidbody()->IsStatic() == true)
        //        //   continue;

        //        //if (glitch->GetRigidbody()->IsStatic() == false && car->GetRigidbody()->IsStatic() == true)
        //        {
        //            //transformA.Translate(details.Normal * (-details.Depth * 0.5f));
        //            //car->GetRigidbody()->AddImpulseForce(details.Normal * (-details.Depth * 0.5f));
        //        }
        //       // else if (glitch->GetRigidbody()->IsStatic() == true && glitch->GetRigidbody()->IsStatic() == false)
        //        {
        //            //transformB.Translate(details.Normal * (details.Depth * 0.5f));
        //            //car->GetRigidbody()->AddImpulseForce(details.Normal * (details.Depth * 0.5f));
        //        }
        //        //else
        //        {   
        //            //transformA.Translate(details.Normal* (details.Depth * -0.5f * 0.5f));
        //            //transformB.Translate(details.Normal* (details.Depth * +0.5f * 0.5f));
        //            //glitch->GetRigidbody()->AddImpulseForce(details.Normal * (details.Depth * -0.5f * 0.5f));
        //            //car->GetRigidbody()->AddImpulseForce(details.Normal * (details.Depth * +0.5f * 0.5f));
        //        }

        //        //car->GetTransform().Translate(details.Normal * (details.Depth * 0.5f));
        //        //car->GetTransform().Translate(details.Normal * (details.Depth * 0.5f));
        //        //glitch->GetTransform().Translate(details.Normal * (details.Depth * -0.5f));
        //    }
        //    else
        //    {
        //        check->GetTransform().SetPosition(glm::vec3(0.0f, -10.0f, 0.0f));
        //        check_two->GetTransform().SetPosition(glm::vec3(0.0f, -10.0f, 0.0f));
        //    }
        //}
        
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
