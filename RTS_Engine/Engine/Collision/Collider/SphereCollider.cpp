#include <BennettPCH.h>
#include <Collision/Collider/SphereCollider.h>
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

	void SphereCollider::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, GetTransform().GetPosition());
		matrix = glm::scale(matrix, glm::vec3(m_Radius));
		renderer.PushConstants.ModelMatrix = matrix;
		renderer.UpdatePushConstants();

		if (GetModel() != nullptr)
		{
			GetModel()->Render(renderer);
		}
	}
}