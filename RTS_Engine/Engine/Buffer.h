#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{
	class Renderer;
	 
	class Buffer
	{
	private:

	protected:
		static bool CreateGenericBuffer(const Renderer& renderer, const VkDeviceSize& deviceSize, const VkBufferUsageFlags& usageFlags, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffers(Renderer& renderer, VkBuffer src, VkBuffer dst, size_t size);

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

		int m_Count;
		
	public:
		int Count() const;
		virtual void Bind(const Renderer& renderer) = 0;
		static uint32_t FindMemoryType(const Renderer& renderer, uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);
	};
}