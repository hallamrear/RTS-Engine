#include "BennettPCH.h"
#include "OBBCollider.h"
#include <Rendering/Renderer.h>
#include <Rendering/Model.h>
#include <Rendering/Mesh.h>

namespace Bennett
{
	OBBCollider::OBBCollider(const Transform& transform, const glm::vec3& extents, const glm::vec3& offset)
		: AABBCollider(transform, extents, offset)
	{
		m_Type = ColliderType::OBB;
		SetModel();
	}

	OBBCollider::~OBBCollider()
	{

	}

	void OBBCollider::UpdateTranslatedCornerPositions()
	{
		const glm::mat4 identity = glm::mat4(1.0f);
		const glm::mat4& parent = GetTransform().GetModelMatrix();
		glm::mat4 movement = glm::translate(identity, GetOffset());
		glm::mat4 scaling = glm::scale(identity, GetExtents());
		glm::mat4 model = (movement * scaling);

		glm::vec4 position = glm::vec4(0.0f);
		for (size_t i = 0; i < 8; i++)
		{
			position = glm::vec4(m_BaseCorners[i], 1.0f);
			m_Corners[i] = parent * model * position;
		}
	}

	void OBBCollider::Render(const Renderer& renderer)
	{
		if (IsEnabled() == false)
			return;

		const Transform& transform = GetTransform();
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(matrix, GetOffset());
		glm::mat4 rotate = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, m_Extent);
		matrix = translate * rotate * scale;
		
		renderer.PushConstants.ModelMatrix = transform.GetModelMatrix() * matrix;
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