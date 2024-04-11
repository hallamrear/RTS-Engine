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
#include <World/MoveableTestEntity.h>

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

Entity* ground = nullptr;
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
        Log(GetLastWin32Error(), LOG_SERIOUS);
        return FALSE;
    }

    if (!InitialiseEngineSystems(*m_Window))
    {
        Log("Failed to initialise engine systems.", LOG_CRITICAL);
        return FALSE;
    }

    AssetManager& am = ServiceLocator::GetAssetManager();

    lines = std::vector<std::pair<glm::vec3, glm::vec3>>();

    std::vector<int> keys =
    {
        BENNETT_MOUSE_LEFT, //Draw cam Line
    };

    inputMonitor = new InputMonitor(keys);

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);


#define TEST_TERRAIN false
#define TEST_OCTREE true

#ifdef TEST_TERRAIN
    if(TEST_TERRAIN)
        InitTestTerrainScene();
#endif

#ifdef TEST_OCTREE
    if (TEST_OCTREE)
        InitTestOctreeScene();
#endif

#ifdef TEST_ENTITIES
    if (TEST_ENTITIES)
        InitTestEntitiesScene();
#endif

  
    return true;
}

void Game::InitTestTerrainScene()
{

}

void Game::InitTestEntitiesScene()
{

}

void Game::InitTestOctreeScene()
{
    AssetManager& am = ServiceLocator::GetAssetManager();

    for (size_t i = 0; i < 100; i++)
    {
        glm::vec3 position = glm::vec3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);
        ground = GetWorld().SpawnEntity(std::to_string(i), glm::vec3(1.0f), position, glm::vec3(0.0f));
        ground->SetModel(am.GetModel("1x1_Cube"));
        ground->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    }

    ground = GetWorld().SpawnEntity("Floor");
    ground->SetModel(am.GetModel("1x1_Cube"));
    ground->GetTransform().SetScale(glm::vec3(20.0f, 1.0f, 20.0f));
    ground->GetTransform().Translate(glm::vec3(0.0f, -5.0f, 0.0f));
    ground->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
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
        float t = glm::radians(90.0f);
        static float s = 0.0f;
        s += dTime * rotSpeed;
        s = fmod(s, 360.0f);

        ServiceLocator::GetRenderer().DrawDebugLine(glm::vec3(0.0f), glm::vec3(5.0f, 0.0f, 0.0f));
        ServiceLocator::GetRenderer().DrawDebugLine(glm::vec3(0.0f), glm::vec3(0.0f, 5.0f, 0.0f));
        ServiceLocator::GetRenderer().DrawDebugLine(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 5.0f));
   
        if (inputMonitor->GetKeyState(BENNETT_MOUSE_LEFT))
        {
            if (pressedLast == false)
            {
                glm::vec3 start = GetCameraController().GetCurrentCamera().GetPosition();
                glm::vec3 end = GetCameraController().GetCurrentCamera().GetForwardVector() * 10.0f;
                lines.push_back(std::make_pair(start, start + end));
            }
            pressedLast = true;
        }
        else
        {
            pressedLast = false;
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
        
        for (size_t i = 0; i < lines.size(); i++)
        {
            ServiceLocator::GetRenderer().DrawDebugLine(
                lines[i].first,
                lines[i].second);
        }

        lTime = cTime;
    }
}
