#pragma once
#include "Buffer.h"
#include "Vertex.h"

namespace Bennett
{
	class BENNETT_ENGINE VertexBuffer : public Buffer
	{
	private:

	public:
		VertexBuffer();
		~VertexBuffer();

		void Bind() override;

		static bool Create(VertexBuffer& buffer, const std::vector<Vertex>& vertices);
		static void Destroy(VertexBuffer& buffer);
	};
}