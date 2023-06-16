#pragma once
#include "Buffer.h"
#include "Renderer.h"

namespace Bennett
{
	class IndexBuffer : public Buffer
	{
	private:

	public:
		void Bind() override;

		static bool Create(IndexBuffer& buffer, const std::vector<unsigned int>& indices);
		static void Destroy(IndexBuffer& buffer);
	};
}