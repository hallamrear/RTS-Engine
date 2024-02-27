#pragma once
#include <System/Transform.h>

namespace Bennett
{
	class Model;
	class Renderer;
	class SphereCollider;
	class AABBCollider;
	class OBBCollider;
	class Ray;

	enum class ColliderType
	{
		Unknown = 0,
		Sphere = 1,
		AABB = 2,
		OBB = 3,
	};

	class BENNETT_ENGINE Collider
	{
	private:
		const Transform& m_Transform;
		glm::vec3 m_Offset;
		virtual void SetType(const ColliderType& type) {};
		Model* m_Model;

	protected:
		ColliderType m_Type = ColliderType::Unknown;
		Collider(const ColliderType& type, const Transform& transform, const glm::vec3& offset = glm::vec3(0.0f));
		Model* GetModel() const;
		void   SetModel();

	public:
		~Collider();

		virtual glm::vec3 GetSupportVertex(const glm::vec3& direction) const = 0;

		const Transform& GetTransform() const;
		const ColliderType& GetType() const { return m_Type; };

		void SetOffset(const glm::vec3& offset) { m_Offset = offset; };
		const glm::vec3& GetOffset() const { return m_Offset; };
			
		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer) = 0;
	};
}