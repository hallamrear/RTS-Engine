#pragma once
#include <Collision/Collider/Collider.h>
#include <Rendering/Renderer.h>

namespace Bennett
{
    class SphereCollider :
        public Collider
    {
    private:
        float m_Radius;
        glm::vec3& m_Position;

    public:
        SphereCollider(glm::vec3& position, const float& radius);
        SphereCollider(glm::vec3& position);
        ~SphereCollider();

        void SetRadius(const float& radius);
        inline const float& GetRadius() const { return m_Radius; };
        inline const glm::vec3& GetPosition() const { return m_Position; };

        void Update(const float& deltaTime);
        void Render(const Renderer& renderer);
    };
}