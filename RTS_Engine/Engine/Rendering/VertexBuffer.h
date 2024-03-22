#pragma once
#include <Rendering/Buffer.h>
#include <Rendering/Vertex.h>

namespace Bennett
{
	class BENNETT_ENGINE VertexBuffer : public Buffer
	{
	private:

	public:
		VertexBuffer();
		~VertexBuffer();

		const size_t& Count() const;

		template<class T>
		static bool Create(VertexBuffer& buffer, const std::vector<T>& vertices)
		{
			bool result = false;

			if (vertices.size() > 0)
			{
				VkBufferCreateInfo bufferInfo{};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = sizeof(T) * vertices.size();
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
		};

		static bool Create(VertexBuffer& buffer, const VkBufferCreateInfo& bufferInfo, void* dataPtr, const size_t& dataCount)
		{
			bool result = false;

			if (dataCount > 0)
			{
				result = Buffer::Create(buffer, bufferInfo, dataPtr);
				buffer.m_Count = dataCount;
			}

			if (!result)
			{
				Log(LOG_MINIMAL, "Failed to create vertex buffer.\n");
			}

			return result;
		};


		void Bind() override;
	};
}