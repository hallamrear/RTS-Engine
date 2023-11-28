#include <BennettPCH.h>
#include <System/ServiceLocator.h>
#include <Rendering/Renderer.h>
#include <Rendering/VertexBuffer.h>

namespace Bennett
{
	VertexBuffer::VertexBuffer() : Buffer()
	{
		m_Count = -1;
	}

	VertexBuffer::~VertexBuffer()
	{
		
	}

	const size_t& VertexBuffer::Count() const
	{
		return m_Count;
	}


	void VertexBuffer::Bind()
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(ServiceLocator::GetRenderer().GetCommandBuffer(), 0, 1, &m_Buffer, offsets);    
	}

	bool VertexBuffer::Create(VertexBuffer& buffer, const std::vector<Vertex>& vertices)
	{
		bool result = false;

		if (vertices.size() > 0)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = sizeof(Vertex) * vertices.size();
			bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			void* data = (void*)vertices.data();
			result = Buffer::Create(buffer, bufferInfo, data);
			data = nullptr;
			buffer.m_Count = vertices.size();
		}

		if (!result)
		{
			Log(LOG_MINIMAL, "Failed to create vertex buffer.\n");
		}
		
		return result;
	}
}