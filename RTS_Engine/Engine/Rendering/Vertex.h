#pragma once
#include <vulkan/vulkan_core.h>

namespace Bennett
{
	class BENNETT_ENGINE Vertex
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
		glm::vec4 Colour;

		Vertex(
			const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& normal = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec2& uv = glm::vec2(0.0f, 0.0f),
			const glm::vec4& colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
		);

		~Vertex();

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescription();
	};
}