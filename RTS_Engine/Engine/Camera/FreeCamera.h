#pragma once
#include "Camera.h"

namespace Bennett
{
    class Window;

    class BENNETT_ENGINE FreeCamera :
        public Camera
    {
	private:
        Window& m_RenderWindow;
        float m_MouseSensitivity;
        glm::vec2 m_MouseDelta;

	public:
        FreeCamera();
        ~FreeCamera();

        glm::mat4 GetViewMatrix();
        void Update(const float& deltaTime);
    };
}   