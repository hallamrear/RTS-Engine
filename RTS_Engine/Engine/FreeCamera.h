#pragma once
#include "Camera.h"

namespace Bennett
{
    class FreeCamera :
        public Camera
    {
	private:

	public:
        FreeCamera();
        ~FreeCamera();

        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix();
        void ProcessInput(const float& deltaTime);
    };
}   