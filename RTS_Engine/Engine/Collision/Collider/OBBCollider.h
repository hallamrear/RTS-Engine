#pragma once
#include <Collision/Collider/AABBCollider.h>

namespace Bennett
{
    class OBBCollider : public AABBCollider
    {
    public:
        OBBCollider(const Transform& transform, glm::vec3 extent);
        ~OBBCollider();
        
        virtual void Update(const float& deltaTime);
        virtual void Render(const Renderer& renderer);
    };
}