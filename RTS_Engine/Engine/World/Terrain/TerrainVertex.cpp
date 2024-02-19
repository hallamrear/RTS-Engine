#include <BennettPCH.h>
#include <World/Terrain/TerrainVertex.h>

namespace Bennett
{
	TerrainVertex::TerrainVertex()
	{
	}
	TerrainVertex::TerrainVertex(const float& height, const float& normalPitch, const float& normalYaw)
	{
		m_Details.x = height;
		m_Details.y = normalPitch;
		m_Details.z = normalYaw;
	}

	TerrainVertex::~TerrainVertex()
	{
		m_Details = glm::vec3(0.0f);
	}

	VkVertexInputBindingDescription TerrainVertex::GetBindingDescription()
	{
		VkVertexInputBindingDescription binding{};
		binding.binding = 0;
		binding.stride = sizeof(TerrainVertex);
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return binding;
	}

	std::vector<VkVertexInputAttributeDescription> TerrainVertex::GetAttributeDescription()
	{
		std::vector<VkVertexInputAttributeDescription> attributes(1);
		//height, pitch, yaw
		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[0].offset = offsetof(TerrainVertex, m_Details);
		return attributes;
	}
}