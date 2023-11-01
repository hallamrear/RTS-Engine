#pragma once
#include "Entity.h"

namespace Bennett
{
    class MoveableTestEntity :
        public Entity
    {
    public:
        MoveableTestEntity(const std::string& name,
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));

        virtual ~MoveableTestEntity();

        virtual void Update(const float& deltaTime);
        virtual void Render(const Renderer& renderer);

    };
}