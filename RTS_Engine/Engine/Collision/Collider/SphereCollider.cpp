#include <BennettPCH.h>
#include <Collision/Collider/SphereCollider.h>
#include <Rendering/Model.h>

namespace Bennett
{
	SphereCollider::SphereCollider(glm::vec3& position, const float& radius)
		: Collider(ColliderType::Sphere), m_Position(position)
	{
		SetRadius(radius / 2.0f);
	}

	SphereCollider::SphereCollider(glm::vec3& position)
		: Collider(ColliderType::Sphere), m_Position(position)
	{
		SetRadius(0.5f);
	}

	SphereCollider::~SphereCollider()
	{
		m_Radius = 0.0f;
	}

	void SphereCollider::SetRadius(const float& radius)
	{
		m_Radius = radius;
	}

	void SphereCollider::Update(const float& deltaTime)
	{
		
	}
	
	void SphereCollider::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, m_Position);
		matrix = glm::scale(matrix, glm::vec3(m_Radius * 2));
		renderer.PushModelMatrix(matrix);

		if (GetModel() != nullptr)
		{
			GetModel()->Render(renderer);
		}
	}
}