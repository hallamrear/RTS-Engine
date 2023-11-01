#pragma once
#include <Camera/Camera.h>

namespace Bennett
{
    class BENNETT_ENGINE ScriptedCamera :
        public Camera
    {
    public:
        ScriptedCamera();
        ~ScriptedCamera();	
        
        void Update(const float& deltaTime);
        glm::mat4 GetViewMatrix();
    };
}

