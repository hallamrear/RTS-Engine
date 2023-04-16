#pragma once
#include "Vertex.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Bennett
{
	class Renderer;

	class Mesh
	{
	private:
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;

		void CreateBuffers(const Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void DestroyBuffers(const Renderer& renderer);

	public:
		Mesh(const Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		~Mesh();

		void Bind(const Renderer& renderer);
		void Render(const Renderer& renderer);
	};
}