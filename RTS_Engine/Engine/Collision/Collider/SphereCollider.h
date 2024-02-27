#pragma once
#include <Collision/Collider/Collider.h>
#include <Rendering/Renderer.h>

namespace Bennett
{
    class BENNETT_ENGINE SphereCollider :
        public Collider
    {
    private:
        float m_Radius;

    public:
        SphereCollider(const Transform& transform, const float& radius = 0.5f, const glm::vec3& offset = glm::vec3(0.0f));
        ~SphereCollider();

        void SetRadius(const float& radius);
        inline const float& GetRadius() const { return m_Radius; };

        glm::vec3 GetSupportVertex(const glm::vec3& direction) const;

        void Render(const Renderer& renderer);
    };
}