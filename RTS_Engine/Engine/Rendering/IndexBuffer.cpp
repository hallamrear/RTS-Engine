#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <Rendering/IndexBuffer.h>
#include <System/ServiceLocator.h>

namespace Bennett
{
    IndexBuffer::IndexBuffer() : Buffer()
    {

    }

    IndexBuffer::~IndexBuffer()
    {

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

            buffer.m_Count = (int)indices.size();
        }

        if (result == FALSE)
        {
            Log(LOG_MINIMAL, "Failed to create index buffer.\n");
        }

        return result;
    }

    const VertexIndex IndexBuffer::GetPrimitiveEndIndex()
    {
        switch (sizeof(VertexIndex))
        {
        case 2: //2 bytes USHORT
            //0xFFFF when indexType is equal to VK_INDEX_TYPE_UINT16.
            return 0xFFFF;
            break;

        case 4: //4 bytes UINT
            //0xFFFFFFFF when the indexType parameter of vkCmdBindIndexBuffer2KHR or vkCmdBindIndexBuffer is equal to VK_INDEX_TYPE_UINT32.            
            return 0xFFFFFFFF;
            break;

        default:
            //0xFF when indexType is equal to VK_INDEX_TYPE_UINT8_KHR.
            Log(LOG_CRITICAL, "Unsupported Vertex Index size.\n");
            break;
        }
    }

    void IndexBuffer::Bind()
    {
        switch (sizeof(VertexIndex))
        {
        case 2: //2 bytes USHORT
            vkCmdBindIndexBuffer(ServiceLocator::GetRenderer().GetCommandBuffer(), m_Buffer, 0, VK_INDEX_TYPE_UINT16);
            break;

        case 4: //4 bytes UINT
            vkCmdBindIndexBuffer(ServiceLocator::GetRenderer().GetCommandBuffer(), m_Buffer, 0, VK_INDEX_TYPE_UINT32);
            break;

        default:
            Log(LOG_CRITICAL, "Unsupported Vertex Index size.\n");
            break;
        }

        //VK_INDEX_TYPE_UINT32
    }
}