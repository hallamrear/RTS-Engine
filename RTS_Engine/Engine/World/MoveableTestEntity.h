#pragma once
#include "Entity.h"
#include <Physics/RigidbodyPhysics.h>

namespace Bennett
{
    class BENNETT_ENGINE MoveableTestEntity :
        public Entity
    {
    private:
        Rigidbody* m_Rigidbody;
        bool m_MovementEnabled;

    public:
        MoveableTestEntity(const std::string& name,
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));

        virtual ~MoveableTestEntity();

        Rigidbody* GetRigidbody() const;

        const bool& IsMovementEnabled() const;
        void SetMovementEnabled(const bool& movementEnabled);

        virtual void Update(const float& deltaTime);
        virtual void Render(const Renderer& renderer);

    };
}