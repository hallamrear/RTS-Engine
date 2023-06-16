#pragma once
#include "Camera.h"

namespace Bennett
{
	class StandardCamera
		: public Camera
	{
	public:
		StandardCamera();
		~StandardCamera();

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
		void ProcessInput(const float& deltaTime);
	};
}