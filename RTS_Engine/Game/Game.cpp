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

std::array<Entity*, 15> axis{};

Entity* corner = nullptr;
AABBCollider* collider;
Entity* ground = nullptr;
Entity* check = nullptr;
Entity* check_two = nullptr;
Entity* tools = nullptr;

//Entity* glitch = nullptr;
//Entity* car = nullptr;
MoveableTestEntity* glitch = nullptr;
MoveableTestEntity* car = nullptr;

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

   /* ground = GetWorld().SpawnEntity("Floor");
    ground->SetModel(am.GetModel("1x1_Cube"));
    ground->GetTransform().SetScale(glm::vec3(20.0f, 1.0f, 20.0f));
    ground->GetTransform().Translate(glm::vec3(0.0f, -5.0f, 0.0f));
    ground->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);    */

    check = GetWorld().SpawnEntity("Check");
    check->GetTransform().SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
    check->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    //check->GetTransform().SetScale(glm::vec3(0.05f));
    check->SetModel(am.GetModel("1x1_Cube"));
    
    check_two = GetWorld().SpawnEntity("Check_2");
    check_two->GetTransform().SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
    check_two->GetTransform().SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
    check_two->GetTransform().SetScale(glm::vec3(0.05f));
    check_two->SetModel(am.GetModel("1x1_Cube"));
    
    car = (MoveableTestEntity*)GetWorld().SpawnTestEntity("shakedown");
    car->GetTransform().SetRotation(glm::vec3(0.0f));
    car->SetModel(am.GetModel("shakedown.gltf"));
    car->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    car->GetModel()->SetTexture(am.GetTexture("shakedown"));
    car->GetTransform().SetPosition(glm::vec3(-2.0f, 0.0f, 2.0f));
    car->GetRigidbody()->SetGravityEnabled(false);
    
    //glitch = (MoveableTestEntity*)GetWorld().SpawnTestEntity("Car4");
    //glitch->SetModel(am.GetModel("Car4.gltf"));
    //glitch->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    //glitch->GetModel()->SetTexture(am.GetTexture("Car4"));
    //glitch->GetTransform().SetPosition(glm::vec3(2.0f, 0.0f, -2.0f));
    //glitch->GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    //glitch->GetRigidbody()->SetGravityEnabled(false);
    //glitch->SetMovementEnabled(false);
    
    //glitch = (MoveableTestEntity*)GetWorld().SpawnTestEntity("Car3");
    //glitch->SetModel(am.GetModel("Car3.gltf"));
    //glitch->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    //glitch->GetModel()->SetTexture(am.GetTexture("Car3"));
    //glitch->GetTransform().SetPosition(glm::vec3(2.0f, 0.0f, 2.0f));
    //glitch->GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    //glitch->GetRigidbody()->SetGravityEnabled(false);
    //glitch->SetMovementEnabled(false);

    GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
    GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);

    return true;
}

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
            printf("capped timestep\n");
            dTime = TIMESTEP_CAP;
        }

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
        

        lTime = cTime;
    }
}
