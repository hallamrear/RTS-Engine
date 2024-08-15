#pragma once
#include <Physics/Collision/Collider/AABBCollider.h>

namespace Bennett
{
    class BENNETT_ENGINE OBBCollider : public AABBCollider
    {
    private:
        virtual void UpdateTranslatedCornerPositions();

    public:
        OBBCollider(const Transform& transform, const glm::vec3& extents, const glm::vec3& offset = glm::vec3(0.0f));
        ~OBBCollider();

        virtual void Render(const Renderer& renderer);
    };
}