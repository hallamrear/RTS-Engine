#include <BennettPCH.h>
#include <Rendering/Mesh.h>
#include <Rendering/Renderer.h>

namespace Bennett
{
	Mesh::Mesh()
	{
		m_Exists = false;
	}
	
	Mesh::~Mesh()
	{
		Destroy();
	}

	void Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<VertexIndex>& indices, 
					  const glm::vec3& max, const glm::vec3& min)
	{
		m_MaxExtent = max;
		m_MinExtent = min;

		bool vbCreated = VertexBuffer::Create(m_VertexBuffer, vertices);
		bool ibCreated = IndexBuffer::Create(m_IndexBuffer, indices);

		if ((vbCreated == false) || (ibCreated == false))
		{
			Log(LOG_SERIOUS, "Failed to create a vertex or index buffer.\n");
			Destroy();
			m_Exists = false;
			return;
		}

		m_Exists = true;
	}

	void Mesh::Destroy()
	{
		if (m_VertexBuffer.Exists())
		{
			VertexBuffer::Destroy(m_VertexBuffer);
		}

		if (m_IndexBuffer.Exists())
		{
			IndexBuffer::Destroy(m_IndexBuffer);
		}
	}

	void Mesh::Render(const Renderer& renderer)
	{
		m_VertexBuffer.Bind();
		m_IndexBuffer.Bind();
		vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), 1, 0, 0, 0);
	}
}