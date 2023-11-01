#include <BennettPCH.h>
#include <Collision/Collider/AABBCollider.h>
#include <Rendering/Model.h>

namespace Bennett
{
	AABBCollider::AABBCollider(glm::vec3& position, glm::vec3 extent)
		: Collider(ColliderType::AABB), m_Position(position)
	{
		m_Extent = extent;
	}

	AABBCollider::~AABBCollider()
	{

	}

	const glm::vec3& AABBCollider::GetPosition() const
	{
		return m_Position;
	}

	const glm::vec3& AABBCollider::GetExtents() const
	{
		return m_Extent;
	}

	void AABBCollider::Update(const float& deltaTime)
	{

	}

	void AABBCollider::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, m_Position);
		matrix = glm::scale(matrix, glm::vec3(m_Extent));
		renderer.PushModelMatrix(matrix);

		if (GetModel() != nullptr)
		{
			GetModel()->Render(renderer);
		}
	}
}