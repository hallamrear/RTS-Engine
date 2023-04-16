#include "BennettPCH.h"
#include "IndexBuffer.h"
#include "Renderer.h"

namespace Bennett
{
    bool IndexBuffer::CreateBuffer(const Renderer& renderer, IndexBuffer& buffer, const std::vector<unsigned int>& indices)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(indices[0]) * indices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        
        if(vkCreateBuffer(renderer.GetDevice(), &bufferInfo, nullptr, &buffer.m_Buffer))
        {
            Log("Failed to create index buffer.", LOG_SERIOUS);
            return false;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(renderer.GetDevice(), buffer.m_Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(renderer, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(renderer.GetDevice(), &allocInfo, nullptr, &buffer.m_BufferMemory) != VK_SUCCESS)
        {
            Log("Failed to allocate memory for index buffer.", LOG_SERIOUS);
            return false;
        }

        vkBindBufferMemory(renderer.GetDevice(), buffer.m_Buffer, buffer.m_BufferMemory, 0);

        void* data;
        vkMapMemory(renderer.GetDevice(), buffer.m_BufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, indices.data(), bufferInfo.size);
        vkUnmapMemory(renderer.GetDevice(), buffer.m_BufferMemory);

        buffer.m_Count = indices.size();

        return true;
	}

	void IndexBuffer::DestroyBuffer(const Renderer& renderer, IndexBuffer& buffer)
	{
		vkDestroyBuffer(renderer.GetDevice(), buffer.m_Buffer, nullptr);
		vkFreeMemory(renderer.GetDevice(), buffer.m_BufferMemory, nullptr);
	}
}