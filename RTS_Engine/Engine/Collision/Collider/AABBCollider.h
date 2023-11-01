#pragma once
#include <Collision/Collider/Collider.h>

namespace Bennett
{
	class AABBCollider :
		public Collider
	{
	private:
		glm::vec3& m_Position;
		glm::vec3  m_Extent;

	public:
		AABBCollider(glm::vec3& position, glm::vec3 extent);
		~AABBCollider();

		const glm::vec3& GetPosition() const;
		const glm::vec3& GetExtents() const;

		inline glm::vec3 GetMinBounds() const
		{
			glm::vec3 halfExtents = m_Extent / 2.0f;
			return m_Position - halfExtents;
		}

		inline glm::vec3 GetMaxBounds() const
		{
			glm::vec3 halfExtents = m_Extent / 2.0f;
			return m_Position + halfExtents;
		};

		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer);
	};
}