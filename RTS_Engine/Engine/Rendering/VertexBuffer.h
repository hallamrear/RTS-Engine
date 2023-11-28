#pragma once
#include <Rendering/Buffer.h>
#include <Rendering/Vertex.h>

namespace Bennett
{
	class BENNETT_ENGINE VertexBuffer : public Buffer
	{
	private:

		size_t m_Count;

	public:
		VertexBuffer();
		~VertexBuffer();

		const size_t& Count() const;

		static bool Create(VertexBuffer& buffer, const std::vector<Vertex>& vertices);
		void Bind() override;
	};
}