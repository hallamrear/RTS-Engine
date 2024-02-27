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

std::array<Entity*, 8> cCornerEntities{};
std::array<Entity*, 8> gCornerEntities{};
AABBCollider* collider;
Entity* glitch = nullptr;
Entity* car = nullptr;
Entity* ground = nullptr;
Entity* check = nullptr;

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

   /* for (size_t i = 0; i < 5; i++)
    {
        axis[i + 0] = GetWorld().SpawnEntity("x_" + std::to_string(i));
        axis[i + 0]->GetTransform().SetPosition(glm::vec3(i, 0.0f, 0.0f));
        axis[i + 0]->GetTransform().SetScale(glm::vec3(0.1f));
        axis[i + 0]->SetModel(am.GetModel("1x1_Cube"));

        axis[i + 1] = GetWorld().SpawnEntity("y_" + std::to_string(i));
        axis[i + 1]->GetTransform().SetPosition(glm::vec3(0.0f, i, 0.0f));;
        axis[i + 1]->GetTransform().SetScale(glm::vec3(0.1f));
        axis[i + 1]->SetModel(am.GetModel("1x1_Cube"));

        axis[i + 2] = GetWorld().SpawnEntity("z_" + std::to_string(i));
        axis[i + 2]->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, i));;
        axis[i + 2]->GetTransform().SetScale(glm::vec3(0.1f));
        axis[i + 2]->SetModel(am.GetModel("1x1_Cube"));
    }*/

    check = GetWorld().SpawnEntity("Check");
    check->GetTransform().SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
    check->SetModel(am.GetModel("1x1_Cube"));
    check->GetModel()->SetTexture(am.GetTexture("red"));

   /* ground = GetWorld().SpawnEntity("Floor");
    ground->SetModel(am.GetModel("1x1_Cube"));
    ground->GetModel()->SetTexture(am.GetTexture("mina"));
    ground->GetTransform().SetScale(glm::vec3(10.0f, 0.5f, 10.0f));
    ground->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);*/

    car = GetWorld().SpawnTestEntity("shakedown");
    car->GetTransform().SetRotationEuler(glm::vec3(0.0f));
    car->SetModel(am.GetModel("shakedown.gltf"));
    car->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    car->GetModel()->SetTexture(am.GetTexture("shakedown"));
    car->GetTransform().SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));

    glitch = GetWorld().SpawnEntity("Glitch");
    glitch->SetModel(am.GetModel("glitch.gltf"));
    glitch->GenerateBroadPhaseColliderFromModel(Bennett::ColliderType::OBB);
    glitch->GetModel()->SetTexture(am.GetTexture("glitch"));
    glitch->GetTransform().SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));

    for (size_t i = 0; i < 8; i++)
    {
        cCornerEntities[i] = GetWorld().SpawnEntity("c_" + std::to_string(i));
        cCornerEntities[i]->SetModel(am.GetModel("1x1_Cube"));
        cCornerEntities[i]->GetTransform().SetScale(glm::vec3(0.125f));
    }

    for (size_t i = 0; i < 8; i++)
    {
        gCornerEntities[i] = GetWorld().SpawnEntity("g_" + std::to_string(i));
        gCornerEntities[i]->SetModel(am.GetModel("1x1_Cube"));
        gCornerEntities[i]->GetTransform().SetScale(glm::vec3(0.125f));
    }

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

        if (car->GetCollider()->GetType()    != ColliderType::Sphere &&
            glitch->GetCollider()->GetType() != ColliderType::Sphere)
        {
            auto c = (AABBCollider*)(car->GetCollider());
            auto corners = c->GetCorners();
            for (size_t i = 0; i < 8; i++)
            {
                cCornerEntities[i]->GetTransform().SetPosition(corners[i]);
                cCornerEntities[i]->GetTransform().SetPosition(corners[i]);
            }

            c = (Bennett::AABBCollider*)(glitch->GetCollider());
            corners = c->GetCorners();
            for (size_t i = 0; i < 8; i++)
            {
                gCornerEntities[i]->GetTransform().SetPosition(corners[i]);
                gCornerEntities[i]->GetTransform().SetPosition(corners[i]);
            }
        }

        float rotSpeed = 10.0f;
        float r = 10.0f;    
        float t = glm::radians(90.0f);
        static float s = 0.0f;
        s += dTime * rotSpeed;
        s = fmod(s, 360.0f);

        if (glitch)
        {
            glitch->GetTransform().SetRotationEuler(glm::vec3(0.0f, s / 4, 0.0f));
            //glitch->GetTransform().SetScale(glm::vec3((sinf(s / 2.0f) * 0.8f) + 1.0f));
        }
        
        if (car)
        {
            /* 
            int axis = rand() % 3;
            
            switch (axis)
            {
             case 0: { car->GetTransform().Rotate(glm::vec3(s / 16, 0.0f, 0.0f)); } break;
             case 1: { car->GetTransform().Rotate(glm::vec3(0.0f, s / 16, 0.0f)); } break;
             case 2: { car->GetTransform().Rotate(glm::vec3(0.0f, 0.0f, s / 16)); } break;

            default:
                break;
            }
            */


            //car->GetTransform().SetScale(glm::vec3((sinf(s / 2.0f) * 0.5f) + 1.0f));
            //car->GetTransform().SetRotationEuler(glm::vec3(0.0f, s / 8, 0.0f));
        }

        if (glitch->GetCollider() != nullptr && car->GetCollider() != nullptr)
        {
            //if (Collision::CheckCollision<ColliderType::OBB, ColliderType::OBB>((const OBBCollider&)*glitch->GetCollider(), (const OBBCollider&)*car->GetCollider()))
            if(GJK::CheckCollision(*(Collider*)glitch->GetCollider(), *(Collider*)car->GetCollider(), &details))
            //if (GJK::CheckCollision(*ground->GetCollider(), *car->GetCollider(), &details))
            {
                Log(LOG_SAFE, "Collision detected.\n");
                Log(LOG_SAFE, "Collision:\nNormal (%f, %f, %f)\nDepth: %f\n", details.Normal.x, details.Normal.y, details.Normal.z, details.Depth);
                check->GetModel()->SetTexture(am.GetTexture("green"));

                car->GetTransform().Translate(details.Normal * (details.Depth * 0.5f));

                //car->GetTransform().Translate(details.Normal * (details.Depth * 0.5f));
                //glitch->GetTransform().Translate(details.Normal * (details.Depth * -0.5f));

            }
            else
            {
                Log(LOG_MINIMAL, "No collision.\n");
                check->GetModel()->SetTexture(am.GetTexture("red"));
            }
        }

        lTime = cTime;
    }
}
