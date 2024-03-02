#pragma once
#include <Rendering/Vertex.h>
#include <Rendering/IndexBuffer.h>
#include <Rendering/VertexBuffer.h>

namespace Bennett
{
	class Renderer;
	class Collider;

	class BENNETT_ENGINE Mesh
	{
	private:
		glm::vec3 m_MaxExtent;
		glm::vec3 m_MinExtent;

		bool m_Exists;
		VertexBuffer m_VertexBuffer;
		IndexBuffer  m_IndexBuffer;

	public:
		Mesh();
		~Mesh();

		const glm::vec3& GetMaxExtents() const { return m_MaxExtent; };
		const glm::vec3& GetMinExtents() const { return m_MinExtent; };

		void Create(const std::vector<Vertex>& vertices, const std::vector<VertexIndex>& indices, const glm::vec3& max = glm::vec3(0.5f, 0.5f, 0.5f), const glm::vec3& min = glm::vec3(-0.5f, -0.5f, -0.5f));
		const bool& Exists() const { return m_Exists; };
		void Destroy();

		void Render(const Renderer& renderer);
	};
}