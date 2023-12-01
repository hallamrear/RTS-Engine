#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{
	class BENNETT_ENGINE TerrainVertex
	{
	private:
		/// <summary>
		/// x = m_Height;
		/// y = m_Pitch;
		/// z = m_Yaw;
		/// </summary>
		glm::vec3 m_Details;

	public:
		TerrainVertex(const float& height, const float& pitch, const float& yaw);
		~TerrainVertex();

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescription();
	};
}