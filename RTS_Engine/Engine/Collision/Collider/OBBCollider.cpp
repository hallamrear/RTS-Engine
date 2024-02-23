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
		
	}

	OBBCollider::~OBBCollider()
	{

	}

	void OBBCollider::UpdateTranslatedCornerPositions()
	{
		const glm::mat4 identity = glm::mat4(1.0f);
		const Transform& transform = GetTransform();
		const glm::vec3 scaledExtents = transform.GetScale() * GetExtents();
		glm::mat4 movement = glm::translate(identity, GetOffset());
		glm::mat4 scaling = glm::scale(identity, scaledExtents);
		glm::mat4 colliderOffset = transform.GetModelMatrix() * movement * scaling;

		glm::vec4 position = glm::vec4(0.0f);
		for (size_t i = 0; i < 8; i++)
		{
			position = glm::vec4(m_BaseCorners[i], 1.0f);
			m_Corners[i] = (colliderOffset) * position;
		}
	}

	void OBBCollider::Render(const Renderer& renderer)
	{
		const Transform& transform = GetTransform();
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(matrix, GetOffset());
		//Do not need to rotate as the rotation is done by the base matrix.
		glm::mat4 rotate = glm::mat4(1.0f); // glm::toMat4(transform.GetRotation());
		glm::mat4 scale = glm::scale(matrix, glm::vec3(m_Extent * transform.GetScale()));
		matrix = translate * rotate * scale;

		renderer.PushConstants.ModelMatrix = transform.GetModelMatrix() * matrix;
		renderer.UpdatePushConstants();

		renderer.SetWireframeGraphicsPipeline();

		if (GetModel() != nullptr)
		{
			GetModel()->Render(renderer);
		}

		renderer.SetSolidGraphicsPipeline();
	}
}