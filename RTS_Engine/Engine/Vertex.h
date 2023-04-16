#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{
	class Vertex
	{
	public:
		glm::vec3 Position;
		glm::vec3 Colour;

		Vertex(const glm::vec3& position, const glm::vec3& colour);
		Vertex(float x, float y, float z);
		~Vertex();

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescription();
	};
}