#pragma once
#include "Buffer.h"
#include "Renderer.h"

namespace Bennett
{
	class IndexBuffer : public Buffer
	{
	private:

	public:
		void Bind(const Renderer& renderer) override
		{
			vkCmdBindIndexBuffer(renderer.GetCommandBuffer(), m_Buffer, 0, VK_INDEX_TYPE_UINT32);
		}

		static bool CreateBuffer(const Renderer& renderer, IndexBuffer& buffer, const std::vector<unsigned int>& indices);
		static void DestroyBuffer(const Renderer& renderer, IndexBuffer& buffer);
	};
}