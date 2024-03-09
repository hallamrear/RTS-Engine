#pragma once
#include <Collision/Collider/Collider.h>
#include <System/Transform.h>

namespace Bennett
{
	class BENNETT_ENGINE AABBCollider :
		public Collider
	{
	protected:
		static std::array<glm::vec3, 8> m_BaseCorners;
		std::array<glm::vec3, 8> m_Corners;
		glm::vec3 m_Extent;

		virtual void UpdateTranslatedCornerPositions();

	public:
		AABBCollider(const Transform& transform, const glm::vec3& extents, const glm::vec3& offset = glm::vec3(0.0f));
		~AABBCollider();

		const std::array<glm::vec3, 8>& GetCorners() const;
		const glm::vec3& GetExtents() const;

		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer);

		virtual glm::vec3 GetSupportVertex(const glm::vec3& direction) const;
	};
}