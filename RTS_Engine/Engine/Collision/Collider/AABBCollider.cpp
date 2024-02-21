#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <glm/gtx/quaternion.hpp>
#include <Collision/Collider/AABBCollider.h>
#include <Rendering/Model.h>

namespace Bennett
{
	AABBCollider::AABBCollider(const Transform& transform, glm::vec3 extent)
		: Collider(ColliderType::AABB), m_Transform(transform)
	{
		m_Extent = extent;
	}

	AABBCollider::~AABBCollider()
	{

	}

	const Transform& AABBCollider::GetTransform() const
	{
		return m_Transform;
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
		glm::mat4 translate = glm::translate(matrix, m_Transform.GetPosition());
		glm::mat4 scale = glm::scale(matrix, glm::vec3(m_Extent * m_Transform.GetScale()));
		matrix = translate * scale;
		renderer.PushConstants.ModelMatrix = matrix;
		renderer.UpdatePushConstants();

		renderer.SetWireframeGraphicsPipeline();

		if (GetModel() != nullptr)
		{
			GetModel()->Render(renderer);
		}

		renderer.SetSolidGraphicsPipeline();
	}
}