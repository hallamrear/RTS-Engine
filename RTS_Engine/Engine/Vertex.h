#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{
	class Vertex
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
		glm::vec4 Colour;

		Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv, const glm::vec4& colour);
		Vertex();
		~Vertex();

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescription();
	};
}