#include <BennettPCH.h>
#include <Defines/MarchingCubesTriangulationTable.h>
#include <System/Manager/AssetManager.h>
#include <System/ServiceLocator.h>
#include <World/Terrain/Terrain.h>
#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>
#include <Rendering/ShaderLoader.h>
#include <stb_perlin.h>

namespace Bennett
{
	Terrain::Terrain(int size) : Entity("Terrain")
	{
		m_Indices = std::vector<VertexIndex>();
		m_Vertices = std::vector<TerrainVertex>();
		m_VertexBuffer = {};
		m_IndexBuffer = {};
		m_TerrainPipeline = {};
	}

	Terrain::~Terrain()
	{

	}

	void Terrain::Generate()
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		const float offset = TriangulationData::CELL_SIZE / TriangulationData::GRID_RESOLUTION;

		std::string src = ServiceLocator::GetResourceFolderLocation();

		if (ShaderLoader::LoadShader(src + "TerrainFragmentShader.spv", "TerrainFragmentShader") == false)
		{
			Log("Failed to load terrain fragment shader.", LOG_MINIMAL);
			return;
		}

		if (ShaderLoader::LoadShader(src + "TerrainVertexShader.spv", "TerrainVertexShader") == false)
		{
			Log("Failed to load terrain vertex shader.", LOG_MINIMAL);
			return;
		}

		CustomPipelineDetails terrainDetails;
		terrainDetails.Cullmode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
		terrainDetails.PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
		terrainDetails.Topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		terrainDetails.InputAttributeDescription = TerrainVertex::GetAttributeDescription();
		terrainDetails.InputBindingDescription = TerrainVertex::GetBindingDescription();
		terrainDetails.PixelShaderName = "TerrainFragmentShader";
		terrainDetails.VertexShaderName = "TerrainVertexShader";

		renderer.CreateCustomPipeline(m_TerrainPipeline, terrainDetails);

		m_Texture = AssetManager::GetTexture("noiseTexture");

		CreateTerrainChunkMesh();

		//todo: this is not offset.
		Log(LOG_SAFE, "Generated %d chunks in terrain.\n", offset);
	}

	void Terrain::CreateTerrainChunkMesh()
	{
		if (m_IndexBuffer.Count() == -1 || m_VertexBuffer.Count() == -1)
		{
			std::vector<TerrainVertex> vertices;

			for (size_t i = 0; i < 160; i++)
			{
				vertices.push_back(TerrainVertex(0.0f, 0.0f, 0.0f));
			}

			size_t size = 4 * 4;
			int height = 4, width = 4;

			VertexBuffer::Create(m_VertexBuffer, vertices);
			//IndexBuffer::Create(m_IndexBuffer, m_Indices);
		}
	}

	Terrain* Terrain::Create(int size)
	{
		Terrain* terrain = new Terrain(size);
		terrain->Generate();
		return terrain;
	}

	void Terrain::Render(const Renderer& renderer)
	{
		renderer.PushModelMatrix(glm::mat4(1.0f));

		size_t chunkCount = 16;

		const CustomPipeline* gp = renderer.GetCurrentGraphicsPipeline();
		renderer.SetCustomGraphicsPipeline(m_TerrainPipeline);
		renderer.PushDescriptorSet(m_Texture);
		renderer.UpdateUniformBuffers();

		m_VertexBuffer.Bind();
		vkCmdDraw(renderer.GetCommandBuffer(), m_VertexBuffer.Count(), 1, 0, 0);
		//m_IndexBuffer.Bind();
		//vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), 8, 0, 0, 0);

		renderer.SetCustomGraphicsPipeline(*gp);
		renderer.SetSolidGraphicsPipeline();
		gp = nullptr;
	}
}