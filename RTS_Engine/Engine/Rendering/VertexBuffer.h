#pragma once
#include <Rendering/Buffer.h>
#include <Rendering/Vertex.h>

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

		//Todo : move to Base buffer object or renderer.
		static void Destroy(VertexBuffer& buffer);
	};
}