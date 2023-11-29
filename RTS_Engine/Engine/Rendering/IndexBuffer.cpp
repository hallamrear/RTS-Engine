#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <Rendering/IndexBuffer.h>
#include <System/ServiceLocator.h>

namespace Bennett
{
    IndexBuffer::IndexBuffer() : Buffer()
    {
        m_Count = -1;
    }

    IndexBuffer::~IndexBuffer()
    {

    }

    void IndexBuffer::Bind()
    {
        vkCmdBindIndexBuffer(ServiceLocator::GetRenderer().GetCommandBuffer(), m_Buffer, 0, VK_INDEX_TYPE_UINT32);
    }

    const size_t& IndexBuffer::Count() const
    {
        return m_Count;
    }

    bool IndexBuffer::Create(IndexBuffer& buffer, const std::vector<VertexIndex>& indices)
    {
        bool result = false;

        if (indices.size() > 0)
        {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = sizeof(VertexIndex) * indices.size();
            bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            void* data = (void*)indices.data();
            result = Buffer::Create(buffer, bufferInfo, data);
            data = nullptr;

            buffer.m_Count = indices.size();
        }

        if (result == FALSE)
        {
            Log(LOG_MINIMAL, "Failed to create index buffer.\n");
        }

        return result;
    }
}