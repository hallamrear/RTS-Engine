#pragma once
#include "Buffer.h"
#include "Vertex.h"

namespace Bennett
{
	class Renderer;

	class VertexBuffer : public Buffer
	{
	private:

	public:
		VertexBuffer();
		~VertexBuffer();

		void Bind(const Renderer& renderer) override;

		static bool CreateBuffer(const Renderer& renderer, VertexBuffer& buffer, const std::vector<Vertex>& vertices);
		static void DestroyBuffer(const Renderer& renderer, VertexBuffer& buffer);
	};
}