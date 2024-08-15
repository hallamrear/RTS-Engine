#include <BennettPCH.h>
#include <Physics/Collision/Collider/SphereCollider.h>
#include <Rendering/Model.h>

namespace Bennett
{
	SphereCollider::SphereCollider(const Transform& transform, const float& radius, const glm::vec3& offset)
		: Collider(ColliderType::Sphere, transform, offset)
	{
		SetRadius(radius);
	}

	SphereCollider::~SphereCollider()
	{
		m_Radius = 0.0f;
	}

	void SphereCollider::SetRadius(const float& radius)
	{
		m_Radius = radius;
	}

	glm::vec3 SphereCollider::GetSupportVertex(const glm::vec3& direction) const
	{
		return GetTransform().GetPosition() + GetOffset() + (glm::normalize(direction) * m_Radius);
	}

	void SphereCollider::Render(const Renderer& renderer)
	{
		const glm::mat4 parent = GetTransform().GetModelMatrix();
		const glm::mat4 identity = glm::mat4(1.0f);
		const glm::mat4 translate = glm::translate(identity, GetOffset());
		const glm::mat4 scale = glm::scale(identity, glm::vec3(m_Radius));
		const glm::mat4 model = translate * scale;

		renderer.PushConstants.ModelMatrix = parent * model;
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
}