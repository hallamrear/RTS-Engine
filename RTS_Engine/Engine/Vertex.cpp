#include "BennettPCH.h"
#include "Vertex.h"

namespace Bennett
{
	Vertex::Vertex(const glm::vec3& position, const glm::vec3& colour)
	{
		Position = position;
		Colour = colour;
	}

	Vertex::Vertex(float x, float y, float z)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;

		Colour.r = 1.0f / (rand() % 255 + 1);
		Colour.g = 1.0f / (rand() % 255 + 1);
		Colour.b = 1.0f / (rand() % 255 + 1);
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

	std::array<VkVertexInputAttributeDescription, 2> Vertex::GetAttributeDescription()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributes{};

		//Position
		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[0].offset = offsetof(Vertex, Position);

		//Colour
		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[1].offset = offsetof(Vertex, Colour);

		return attributes;
	}
}