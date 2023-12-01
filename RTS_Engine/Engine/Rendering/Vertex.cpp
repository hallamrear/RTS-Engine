#include <BennettPCH.h>
#include <Rendering/Vertex.h>

namespace Bennett
{
	Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv, const glm::vec4& colour)
	{
		Position = position;
		Normal = normal;
		UV = uv;
		Colour = colour;
	}

	Vertex::~Vertex()
	{

	}

	VkVertexInputBindingDescription Vertex::GetBindingDescription()
	{
		VkVertexInputBindingDescription binding{};
		binding.binding = 0;
		binding.stride = sizeof(Vertex);
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return binding;
	}

	std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescription()
	{
		std::vector<VkVertexInputAttributeDescription> attributes(4);

		//Position
		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[0].offset = offsetof(Vertex, Position);

		//Normal
		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[1].offset = offsetof(Vertex, Normal);

		//UV
		attributes[2].binding = 0;
		attributes[2].location = 2;
		attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributes[2].offset = offsetof(Vertex, UV);

		//Colour
		attributes[3].binding = 0;
		attributes[3].location = 3;
		attributes[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributes[3].offset = offsetof(Vertex, Colour);

		return attributes;
	}
}