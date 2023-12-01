#pragma once
#include <vulkan/vulkan.h>
#include <Rendering/Vertex.h>

namespace Bennett
{
	struct BENNETT_ENGINE CustomPipelineDetails
	{
	public:
		std::string VertexShaderName = "DefaultVertex";
		std::string PixelShaderName = "DefaultFragment";
		VkPrimitiveTopology Topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		VkFrontFace FrontFace = VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
		VkPolygonMode PolygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
		VkCullModeFlagBits Cullmode = VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
		float LineWidth = 2.0f;
		VkVertexInputBindingDescription InputBindingDescription = Vertex::GetBindingDescription();
		std::vector<VkVertexInputAttributeDescription> InputAttributeDescription = Vertex::GetAttributeDescription();
	};

	class CustomPipeline
	{
	private:
		friend class Renderer;
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;

	public:
		
		inline CustomPipeline()
		{
			m_Pipeline = VK_NULL_HANDLE;
			m_Layout = VK_NULL_HANDLE;
		};

		inline ~CustomPipeline()
		{
			//if hit assert, Not properly cleaned up a pipeline
			//assert(m_Pipeline != VK_NULL_HANDLE && m_Layout != VK_NULL_HANDLE);
		}
	};
}