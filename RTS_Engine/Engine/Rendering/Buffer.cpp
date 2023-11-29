#include <BennettPCH.h>
#include <System/ServiceLocator.h>
#include <Rendering/Renderer.h>
#include <Rendering/Buffer.h>
#include <Rendering/Vertex.h>

namespace Bennett
{
	Buffer::Buffer()
	{
		m_Buffer = VkBuffer{};
		m_BufferMemory = VkDeviceMemory{};
		m_Count = -1;
	}

	Buffer::~Buffer()
	{
		//todo : destroy
	}

	VkBuffer& Buffer::Object()
	{
		return m_Buffer;
	}

	VkDeviceMemory& Buffer::Memory()
	{
		return m_BufferMemory;
	}

	void Buffer::Bind()
	{
		return;
	}

	uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(ServiceLocator::GetRenderer().GetPhysicalDevice(), &memoryProperties);

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) &&
				(memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) ==
				propertyFlags
				)
				return i;
		}

		Log("Failed to find a suitable memory type", LOG_CRITICAL);
		return 0;
	}
	
	bool Buffer::Create(Buffer& buffer, const VkBufferCreateInfo& createInfo, void* bufferData)
	{
		size_t bufferSize = createInfo.size;

		if (bufferSize <= 0)
		{
			Log("Invalid buffer data.", LOG_MINIMAL);
			return false;
		}

		Renderer& renderer = ServiceLocator::GetRenderer();

		if (vkCreateBuffer(renderer.GetDevice(), &createInfo, nullptr, &buffer.m_Buffer) != VK_SUCCESS)
		{
			Log("Failed to create buffer.", LOG_SERIOUS);
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(renderer.GetDevice(), buffer.m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(renderer.GetDevice(), &allocInfo, nullptr, &buffer.m_BufferMemory) != VK_SUCCESS)
		{
			Log("Failed to allocate memory for buffer.", LOG_SERIOUS);
			return false;
		}

		vkBindBufferMemory(renderer.GetDevice(), buffer.m_Buffer, buffer.m_BufferMemory, 0);

		if (bufferData != nullptr)
		{
			//Map data to buffer memory
			void* data;
			vkMapMemory(renderer.GetDevice(), buffer.m_BufferMemory, 0, createInfo.size, 0, &data);
			memcpy(data, bufferData, createInfo.size);
			vkUnmapMemory(renderer.GetDevice(), buffer.m_BufferMemory);
		}

		return true;
	}

	void Buffer::Destroy(Buffer& buffer)
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		vkDestroyBuffer(renderer.GetDevice(), buffer.m_Buffer, nullptr);
		vkFreeMemory(renderer.GetDevice(), buffer.m_BufferMemory, nullptr);
		buffer.m_Count = -1;
	}

	void Buffer::Copy(VkBuffer& src, const VkBuffer& dst, const size_t& size)
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		VkCommandBuffer cmd = renderer.BeginSingleTimeCommands();
		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(cmd, src, dst, 1, &copyRegion);
		renderer.EndSingleTimeCommands(cmd);
	}
}