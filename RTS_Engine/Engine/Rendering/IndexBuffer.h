#pragma once
#include <Rendering/Buffer.h>

namespace Bennett
{
	typedef unsigned int VertexIndex;

	class BENNETT_ENGINE IndexBuffer : public Buffer
	{
	private:
		size_t m_Count;

	public:
		IndexBuffer();
		~IndexBuffer();

		const size_t& Count() const;

		static bool Create(IndexBuffer& buffer, const std::vector<VertexIndex>& indices);
		void Bind() override;
	};
}