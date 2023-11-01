#include <BennettPCH.h>
#include <System/ServiceLocator.h>
#include <Rendering/Renderer.h>
#include <Rendering/VertexBuffer.h>

namespace Bennett
{
	VertexBuffer::VertexBuffer() : Buffer()
	{
		
	}

	VertexBuffer::~VertexBuffer()
	{
		
	}

	void VertexBuffer::Bind()
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(ServiceLocator::GetRenderer().GetCommandBuffer(), 0, 1, &m_Buffer, offsets);
	}

	bool VertexBuffer::Create(VertexBuffer& buffer, const std::vector<Vertex>& vertices)
	{
		if (vertices.size() <= 0)
			return false;

		Renderer& renderer = ServiceLocator::GetRenderer();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(vertices[0]) * vertices.size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(renderer.GetDevice(), &bufferInfo, nullptr, &buffer.m_Buffer) != VK_SUCCESS)
		{
			Log("Failed to create vertex buffer", LOG_SERIOUS);
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
			Log("Failed to allocate memory for vertex buffer.", LOG_SERIOUS);
			return false;
		}

		vkBindBufferMemory(renderer.GetDevice(), buffer.m_Buffer, buffer.m_BufferMemory, 0);

		//Map data to buffer memory
		void* data;
		vkMapMemory(renderer.GetDevice(), buffer.m_BufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, vertices.data(), bufferInfo.size);
		vkUnmapMemory(renderer.GetDevice(), buffer.m_BufferMemory);

		buffer.m_Count = (int)vertices.size();

		return true;
	}

	void VertexBuffer::Destroy(VertexBuffer& buffer)
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		vkDestroyBuffer(renderer.GetDevice(), buffer.m_Buffer, nullptr);
		vkFreeMemory(renderer.GetDevice(), buffer.m_BufferMemory, nullptr);
		buffer.m_Count = -1;
	}
}