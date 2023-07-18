#pragma once
#include "Camera.h"

namespace Bennett
{
    class BENNETT_ENGINE ScriptedCamera :
        public Camera
    {
    public:
        ScriptedCamera();
        ~ScriptedCamera();	
        
        void ProcessInput(const float& deltaTime);
        glm::mat4 GetViewMatrix();
    };
}

