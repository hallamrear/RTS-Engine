#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{	 
	class Buffer
	{
	private:

	protected:
		static bool CreateGenericBuffer(const VkDeviceSize& deviceSize, const VkBufferUsageFlags& usageFlags, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffers(VkBuffer src, VkBuffer dst, size_t size);

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		int m_Count;
		
	public:
		int Count() const;
		virtual void Bind() = 0;
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);
	};
}