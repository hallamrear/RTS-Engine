#include "BennettPCH.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Vertex.h"

namespace Bennett
{
	uint32_t Buffer::FindMemoryType(const Renderer& renderer, uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(renderer.GetPhysicalDevice(), &memoryProperties);

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) &&
				(memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) ==
				propertyFlags
				)
				return i;
		}

		Log("Failed to find a suitable memory type", LOG_CRITICAL);
	}

	bool Buffer::CreateGenericBuffer(
		const Renderer& renderer,
		const VkDeviceSize& deviceSize,
		const VkBufferUsageFlags& usageFlags, 
		const VkMemoryPropertyFlags& properties, 
		VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		//Create buffer object
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = deviceSize;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(renderer.GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			Log("Failed to create vertex buffer.", LOG_SERIOUS);
			return false;
		}

		//Allocate memory
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(renderer.GetDevice(), buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = Buffer::FindMemoryType(renderer, memoryRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(renderer.GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		{
			Log("Failed to allocate memory for a vertex buffer.", LOG_CRITICAL);
			return false;
		}

		//Filling the buffer memory.
		vkBindBufferMemory(renderer.GetDevice(), buffer, bufferMemory, 0);
		return true; 
	}

	void Buffer::CopyBuffers(Renderer& renderer, VkBuffer src, VkBuffer dst, size_t size)
	{
		VkCommandBuffer cmd = renderer.BeginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(cmd, src, dst, 1, &copyRegion);

		renderer.EndSingleTimeCommands(cmd);
	}

	int Buffer::Count() const
	{
		return m_Count;
	}
}