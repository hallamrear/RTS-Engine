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
		const Transform& transform = GetTransform();
		const glm::vec3& colliderOffset = GetOffset();
		const glm::vec3& colliderExtent = GetExtents();
		const glm::vec3 scaledExtents = transform.GetScale() * colliderExtent;

		for (size_t i = 0; i < 8; i++)
		{
			glm::vec3 rotatedPoint = glm::rotate(transform.GetRotation(), m_BaseCorners[i]);
			//rotatedPoint = glm::translate(rotatedPoint, scaledExtents);
			m_Corners[i] = transform.GetPosition() + colliderOffset + (rotatedPoint * scaledExtents);
		}
	}

	void OBBCollider::Render(const Renderer& renderer)
	{
		const Transform& transform = GetTransform();
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(matrix, transform.GetPosition() + GetOffset());
		glm::mat4 rotate = glm::toMat4(transform.GetRotation());
		glm::mat4 scale = glm::scale(matrix, glm::vec3(m_Extent * transform.GetScale()));
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