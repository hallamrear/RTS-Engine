#pragma once
#include "Camera.h"

namespace Bennett
{
	class BENNETT_ENGINE StandardCamera
		: public Camera
	{
	public:
		StandardCamera();
		~StandardCamera();

		glm::mat4 GetViewMatrix();
		void Update(const float& deltaTime);
	};
}