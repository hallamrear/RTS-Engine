#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <glm/gtx/quaternion.hpp>
#include <Physics/Collision/Collider/AABBCollider.h>
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
		const glm::mat4 identity = glm::mat4(1.0f);
		const glm::mat4 pTransform = glm::translate(identity, GetTransform().GetPosition());
		const glm::mat4 pScale = glm::scale(identity, GetTransform().GetScale());
		const glm::mat4 parent = pTransform * pScale;

		const glm::mat4 translate = glm::translate(identity, GetOffset());
		const glm::mat4 scale = glm::scale(identity, GetExtents());
		const glm::mat4 model = translate * scale;

		for (size_t i = 0; i < 8; i++)
		{
			m_Corners[i] = parent * model * glm::vec4(m_BaseCorners[i], 1.0f);
		}
	}

	void AABBCollider::Update(const float& deltaTime)
	{
		UpdateTranslatedCornerPositions();
	}

	void AABBCollider::Render(const Renderer& renderer)
	{
		const glm::mat4 identity = glm::mat4(1.0f);
		const glm::mat4 pTransform = glm::translate(identity, GetTransform().GetPosition());
		const glm::mat4 pScale = glm::scale(identity, GetTransform().GetScale());
		const glm::mat4 parent = pTransform * pScale;

		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(matrix, GetOffset());
		glm::mat4 scale = glm::scale(matrix, glm::vec3(m_Extent));
		matrix = translate * scale;

		renderer.PushConstants.ModelMatrix = parent * matrix;
		renderer.UpdatePushConstants();

		if (ENABLE_DRAW_COLLIDERS_OUTLINE)
		{
			renderer.SetWireframeGraphicsPipeline();

			if (GetModel() != nullptr)
			{
				GetModel()->Render(renderer);
			}

			renderer.SetSolidGraphicsPipeline();
		}
	}

	glm::vec3 AABBCollider::GetSupportVertex(const glm::vec3& direction) const
	{
		glm::vec3 maxPoint;
		float maxDistance = -FLT_MAX;

		for (int i = 0; i < 8; i++)
		{
			float distance = glm::dot(m_Corners[i], direction);
			if (distance >= maxDistance)
			{
				maxDistance = distance;
				maxPoint = m_Corners[i];
			}
		}

		return maxPoint;
	}
}