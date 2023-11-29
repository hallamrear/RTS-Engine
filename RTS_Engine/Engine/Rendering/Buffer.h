#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{	 
	class BENNETT_ENGINE Buffer
	{
	private:
		
	protected:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		int m_Count;
		
	public:
		Buffer();
		~Buffer();

		VkBuffer& Object();
		VkDeviceMemory& Memory();

		virtual void Bind();
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

		static void Copy(VkBuffer& src, const VkBuffer& dst, const size_t& size);
		static bool Create(Buffer& buffer, const VkBufferCreateInfo& createInfo, void* bufferData);
		static void Destroy(Buffer& buffer);
	};
}