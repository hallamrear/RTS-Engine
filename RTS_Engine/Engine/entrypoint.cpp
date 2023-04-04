#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "BennettPCH.h"
#include "Application.h"
#include "Logger.h"

using namespace Bennett;

extern Application* CreateApplication(int, char**, WindowDetails);

int main(int argc, char **argv)
{
	WindowDetails details = WindowDetails();
	details.Title = "Yet another engine but in Vulkan!";

	auto app = Bennett::CreateApplication(argc, argv, details);

	if (!app)
	{
		Log("Application Run finished. Application deleting.", LOG_CRITICAL);
		return 0;
	}

	app->GameLoop();

	Log("Application has finished running, it is now closing.", LOG_SAFE);
	delete app;
	app = nullptr;
}
