#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{	 
	class BENNETT_ENGINE Buffer
	{
	private:
		
	protected:
		Buffer();
		~Buffer();

		static bool CreateGenericBuffer(const VkDeviceSize& deviceSize, const VkBufferUsageFlags& usageFlags, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffers(VkBuffer src, VkBuffer dst, size_t size);

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		int m_Count;
		
	public:
		virtual void Bind() = 0;
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

		static bool Create(Buffer& buffer, const VkBufferCreateInfo& createInfo, void* bufferData);
		static void Destroy(Buffer& buffer);
	};
}