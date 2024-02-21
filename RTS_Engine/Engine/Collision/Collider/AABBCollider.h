#pragma once
#include <Collision/Collider/Collider.h>
#include <System/Transform.h>

namespace Bennett
{
	class AABBCollider :
		public Collider
	{
	protected:
		const Transform& m_Transform;
		glm::vec3  m_Extent;

	public:
		AABBCollider(const Transform& transform, glm::vec3 extent);
		~AABBCollider();

		const Transform& GetTransform() const;
		const glm::vec3& GetExtents() const;

		inline glm::vec3 GetMinBounds() const
		{
			glm::vec3 halfExtents = m_Extent / 2.0f;
			return m_Transform.GetPosition() - halfExtents;
		}

		inline glm::vec3 GetMaxBounds() const
		{
			glm::vec3 halfExtents = m_Extent / 2.0f;
			return m_Transform.GetPosition() + halfExtents;
		};

		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer);
	};
}