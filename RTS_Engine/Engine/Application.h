#pragma once
#include "Entity.h"
#include "World.h"
#include "CameraController.h"
#include "WindowDetails.h"

class Application
{
private:
	Bennett::World m_World;
	Bennett::CameraController& m_CameraController;
	Bennett::InputMonitor* m_ApplicationControls;

	bool m_IsRunning;

	bool InitialiseServices();
	void ShutdownServices();

	void ProcessInput(const float& DeltaTime);
	void Update(float DeltaTime);
	void Render();

public:
	Application();
	~Application();

	void GameLoop();

	bool Initialise(int argc, char** argv, const Bennett::WindowDetails& details);
	void Destroy();
};

Application* CreateApplication(int argc, char** argv, const Bennett::WindowDetails& details);