#include <BennettPCH.h>
#include <Rendering/Mesh.h>

namespace Bennett
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		if (VertexBuffer::Create(m_VertexBuffer, vertices) == false)
		{
			Log("Failed to create a vertex buffer.", LOG_STATUS::LOG_SERIOUS);
		}

		if (IndexBuffer::Create(m_IndexBuffer, indices) == false)
		{
			Log("Failed to create a index buffer.", LOG_STATUS::LOG_SERIOUS);
		}
	}

	Mesh::~Mesh()
	{
		VertexBuffer::Destroy(m_VertexBuffer);
		IndexBuffer::Destroy(m_IndexBuffer);
	}

	void Mesh::Render(const Renderer& renderer)
	{
		m_VertexBuffer.Bind();
		m_IndexBuffer.Bind();
		vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), 1, 0, 0, 0);
	}
}