#pragma once
#include "Camera.h"

namespace Bennett
{
    class BENNETT_ENGINE FreeCamera :
        public Camera
    {
	private:
        glm::vec2 m_MousePositionLastFrame;
        bool m_IsMouseLocked;
        float m_MouseSensitivity;

	public:
        FreeCamera();
        ~FreeCamera();

        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix();
        void ProcessInput(const float& deltaTime);
    };
}   