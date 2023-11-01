#pragma once
#include <Rendering/Vertex.h>
#include <Rendering/IndexBuffer.h>
#include <Rendering/VertexBuffer.h>

namespace Bennett
{
	class Renderer;

	class BENNETT_ENGINE Mesh
	{
	private:
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;

		void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void DestroyBuffers();

	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		~Mesh();

		void Bind();
		void Render(const Renderer& renderer);
	};
}