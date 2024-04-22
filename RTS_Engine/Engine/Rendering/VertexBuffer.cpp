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

	const size_t& VertexBuffer::Count() const
	{
		return m_Count;
	}

	void VertexBuffer::Bind()
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(ServiceLocator::GetRenderer().GetCommandBuffer(), 0, 1, &m_Buffer, offsets);    
	}
}