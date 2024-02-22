#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <glm/gtx/quaternion.hpp>
#include <Collision/Collider/AABBCollider.h>
#include <Rendering/Model.h>

namespace Bennett
{
	std::array<glm::vec3, 8> AABBCollider::m_BaseCorners =
	{
		glm::vec3(+0.5f, 0.5f,-0.5f),
		glm::vec3(+0.5f,-0.5f,-0.5f),
		glm::vec3(+0.5f, 0.5f, 0.5f),
		glm::vec3(+0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f)
	};

	AABBCollider::AABBCollider(const Transform& transform, const glm::vec3& extents, const glm::vec3& offset)
		: Collider(ColliderType::AABB, transform, offset)
	{
		m_Extent = extents;
		UpdateTranslatedCornerPositions();
	}

	AABBCollider::~AABBCollider()
	{

	}

	const glm::vec3& AABBCollider::GetExtents() const
	{
		return m_Extent;
	}

	const std::array<glm::vec3, 8>& AABBCollider::GetCorners() const
	{
		return m_Corners;
	}

	void AABBCollider::UpdateTranslatedCornerPositions()
	{
		const glm::vec3 entityPosition = GetTransform().GetPosition();
		const glm::vec3& entityScale = GetTransform().GetScale();
		const glm::vec3& colliderOffset = GetOffset();
		const glm::vec3& colliderExtent = GetExtents();

		const glm::vec3 scaledExtents = entityScale * colliderExtent;

		for (size_t i = 0; i < 8; i++)
		{
			m_Corners[i] = entityPosition + colliderOffset + m_BaseCorners[i] * scaledExtents;
		}
	}

	void AABBCollider::Update(const float& deltaTime)
	{
		UpdateTranslatedCornerPositions();
	}

	void AABBCollider::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(matrix, GetTransform().GetPosition() + GetOffset());
		glm::mat4 scale = glm::scale(matrix, glm::vec3(m_Extent * GetTransform().GetScale()));
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