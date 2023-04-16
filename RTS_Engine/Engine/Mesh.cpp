#include "BennettPCH.h"
#include "Mesh.h"

namespace Bennett
{
	Mesh::Mesh(const Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		CreateBuffers(renderer, vertices, indices);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Bind(const Renderer& renderer)
	{
		m_VertexBuffer.Bind(renderer);
		m_IndexBuffer.Bind(renderer);
	}

	void Mesh::CreateBuffers(const Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		if (VertexBuffer::CreateBuffer(renderer, m_VertexBuffer, vertices) == false)
			Log("Failed to create a vertex buffer.", LOG_STATUS::LOG_SERIOUS);

		if (IndexBuffer::CreateBuffer(renderer, m_IndexBuffer, indices) == false)
			Log("Failed to create a vertex buffer.", LOG_STATUS::LOG_SERIOUS);
		;
	}

	void Mesh::DestroyBuffers(const Renderer& renderer)
	{
		VertexBuffer::DestroyBuffer(renderer, m_VertexBuffer);
		IndexBuffer::DestroyBuffer(renderer, m_IndexBuffer);
	}

	void Mesh::Render(const Renderer& renderer)
	{
		vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), 1, 0, 0, 0);
	}
}