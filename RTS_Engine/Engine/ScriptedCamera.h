#pragma once
#include "Camera.h"

namespace Bennett
{
    class ScriptedCamera :
        public Camera
    {
    public:
        ScriptedCamera();
        ~ScriptedCamera();	
        
        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix();
        void ProcessInput(const float& deltaTime);
    };
}

