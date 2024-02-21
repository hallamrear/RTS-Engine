#include "BennettPCH.h"
#include "OBBCollider.h"
#include <Rendering/Renderer.h>
#include <Rendering/Model.h>

namespace Bennett
{
	OBBCollider::OBBCollider(const Transform& transform, glm::vec3 extent)
		: AABBCollider(transform, extent)
	{

	}

	OBBCollider::~OBBCollider()
	{

	}

	void OBBCollider::Update(const float& deltaTime)
	{

	}

	void OBBCollider::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(matrix, m_Transform.GetPosition());
		glm::mat4 rotate = glm::toMat4(m_Transform.GetRotation());
		glm::mat4 scale = glm::scale(matrix, glm::vec3(m_Extent * m_Transform.GetScale()));
		matrix = translate * rotate * scale;

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