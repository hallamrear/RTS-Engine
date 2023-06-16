#include "BennettPCH.h"
#include "Mesh.h"

namespace Bennett
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		CreateBuffers(vertices, indices);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Bind()
	{
		if(m_VertexBuffer.Count() > 0)
			m_VertexBuffer.Bind();

		if(m_IndexBuffer.Count() > 0)
			m_IndexBuffer.Bind();
	}

	void Mesh::CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		if (VertexBuffer::Create(m_VertexBuffer, vertices) == false)
			Log("Failed to create a vertex buffer.", LOG_STATUS::LOG_SERIOUS);

		if (IndexBuffer::Create(m_IndexBuffer, indices) == false)
			Log("Failed to create a index buffer.", LOG_STATUS::LOG_SERIOUS);
		
	}

	void Mesh::DestroyBuffers()
	{
		VertexBuffer::Destroy(m_VertexBuffer);
		IndexBuffer::Destroy(m_IndexBuffer);
	}

	void Mesh::Render(const Renderer& renderer)
	{
		if (m_IndexBuffer.Count() > 0)
			vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), 1, 0, 0, 0);
		else
			vkCmdDraw(renderer.GetCommandBuffer(), m_VertexBuffer.Count(), 1, 0, 0);
	}
}