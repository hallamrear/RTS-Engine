#pragma once

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
		ColliderType m_Type = ColliderType::Unknown;
		Model* m_Model;
		void SetModel();

	protected:
		Collider(const ColliderType& type);
		Model* GetModel() const;

	public:
		~Collider();
			
		virtual void Update(const float& deltaTime)   = 0;
		virtual void Render(const Renderer& renderer) = 0;
	};
}