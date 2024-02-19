#include <BennettPCH.h>
#include <Defines/MarchingCubesTriangulationTable.h>
#include <System/Manager/AssetManager.h>
#include <System/ServiceLocator.h>
#include <World/Terrain/Terrain.h>
#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>
#include <Rendering/ShaderLoader.h>
#include <cmath>
#include <External/PerlinNoise.hpp>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

namespace Bennett
{
	Terrain::Terrain() : Entity("Terrain")
	{
		m_Texture = nullptr;
		m_Indices = std::vector<VertexIndex>();
		m_Vertices = std::vector<TerrainVertex>();
		m_VertexBuffer = {};
		m_IndexBuffer = {};
		m_TerrainPipeline = {};

		for (size_t i = 0; i < TERRAIN_CHUNK_COUNT; i++)
		{
			m_ChunkLocations[i] = ChunkPosition();
		}
	}

	Terrain::~Terrain()
	{

	}

	void Terrain::Generate(const long& seed)
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		siv::PerlinNoise perlin;

		for (int i = 0; i < TERRAIN_CHUNK_COUNT; i++)
		{
			int x = i / TERRAIN_WIDTH;
			int z = i % TERRAIN_WIDTH;
			m_ChunkLocations[i].x = (double)(x * TERRAIN_CELL_SIZE);
			m_ChunkLocations[i].y = (double)(z * TERRAIN_CELL_SIZE);

			int gridX = i / (int)TERRAIN_CELL_SIZE;
			int gridZ = i % (int)TERRAIN_CELL_SIZE;
			glm::vec2 gridPosition = GridPoints[gridX][gridZ];

			glm::vec3 samplePosition = glm::vec3(m_ChunkLocations[i].x + gridPosition.x, 0.0f, m_ChunkLocations[i].y + gridPosition.y);
			double height = GetPointHeight(samplePosition);
			samplePosition.y = height;
		}	
		
		size_t size = sizeof(ChunkPosition) * TERRAIN_CHUNK_COUNT;
		memcpy(&renderer.UniformMatrixBuffer.TerrainChunkLocations, &m_ChunkLocations, size);

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
		terrainDetails.Cullmode = VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
		terrainDetails.PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
		terrainDetails.Topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		terrainDetails.InputAttributeDescription = TerrainVertex::GetAttributeDescription();
		terrainDetails.InputBindingDescription = TerrainVertex::GetBindingDescription();
		terrainDetails.PixelShaderName = "TerrainFragmentShader";
		terrainDetails.VertexShaderName = "TerrainVertexShader";

		renderer.CreateCustomPipeline(m_TerrainPipeline, terrainDetails);

		m_Texture = AssetManager::GetTexture("mina");
		m_Texture = AssetManager::GetTexture("noiseTexture");
		m_Texture = AssetManager::GetTexture("noiseTexture2");

		CreateTerrainChunkMesh();

		//todo: this is not offset.
		Log(LOG_SAFE, "Generated %d chunks in terrain.\n", TERRAIN_CHUNK_COUNT);
	}

	double Terrain::GetPointHeight(const glm::vec3& position)
	{
		return 0.0;

		//perlin.noise2D(samplePosition.x, samplePosition.y);
	}

	void Terrain::CreateTerrainChunkMesh()
	{
		if (m_VertexBuffer.Count() == -1 && m_IndexBuffer.Count() == -1)
		{
			std::vector<TerrainVertex> vertices(160, TerrainVertex());
			//vertices.resize(160);
			std::vector<unsigned int> indices;

			/*for (size_t i = 0; i < 160; i++)
			{
				vertices.push_back(TerrainVertex(0.0f, 0.0f, 0.0f));
			}*/

			VertexBuffer::Create(m_VertexBuffer, vertices);
		}
	}

	/*
	void Terrain::CreateTerrainChunkMesh()
	{
		if (m_VertexBuffer.Count() == -1)
		{
			std::vector<TerrainVertex> vertices;

			for (size_t i = 0; i < 160; i++)
			{
				vertices.push_back(TerrainVertex(0.0f, 0.0f, 0.0f));
			}

			VertexBuffer::Create(m_VertexBuffer, vertices);
		}
	}
	*/

	Terrain* Terrain::Create(const long& seed)
	{
		Terrain* terrain = new Terrain();
		terrain->Generate(seed);
		return terrain;
	}

	void Terrain::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, GetScale());
		glm::mat4 rotate = glm::toMat4(GetRotation());
		glm::vec3 pos = GetPosition();
		glm::mat4 translate = glm::translate(matrix, pos);
		matrix = translate * rotate * scale;
		renderer.PushConstants.ModelMatrix = matrix;
		renderer.UpdatePushConstants();

		const CustomPipeline* gp = renderer.GetCurrentGraphicsPipeline();
		renderer.SetCustomGraphicsPipeline(m_TerrainPipeline);
		renderer.PushDescriptorSet(m_Texture);
		size_t size = sizeof(ChunkPosition) * TERRAIN_CHUNK_COUNT;
		memcpy(renderer.UniformMatrixBuffer.TerrainChunkLocations, m_ChunkLocations, size);
 		renderer.UpdateUniformBuffers();
		m_VertexBuffer.Bind();
		vkCmdDraw(renderer.GetCommandBuffer(), m_VertexBuffer.Count(), TERRAIN_CHUNK_COUNT, 0, 0);
		//vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), TERRAIN_CHUNK_COUNT, 0, 0, 0);

		renderer.SetCustomGraphicsPipeline(*gp);
		renderer.SetSolidGraphicsPipeline();
		gp = nullptr;
	}
}