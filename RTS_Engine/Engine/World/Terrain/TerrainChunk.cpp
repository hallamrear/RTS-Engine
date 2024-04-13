#include "BennettPCH.h"
#include "TerrainChunk.h"
#include <Rendering/Renderer.h>
#include <System/ServiceLocator.h>
#include <System/Manager/AssetManager.h>
#include <Rendering/Texture.h>
#include <World/WorldChunk.h>
#include <External/PerlinNoise.hpp>

namespace Bennett
{

	TerrainChunk::TerrainChunk(const glm::vec3& position)
	{
		m_Position = position;
		m_Texture = ServiceLocator::GetAssetManager().GetAsset<Texture>("red");

		std::array<int, ChunkCellCount> data = { 0 };
		CreateChunk(data);

		CustomPipelineDetails details;
		details.PrimitiveRestartEnabled = true;
		details.Topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		details.PolygonMode = VK_POLYGON_MODE_FILL;

		ServiceLocator::GetRenderer().CreateCustomPipeline(m_TerrainPipeline, details);
	}

	TerrainChunk::~TerrainChunk()
	{
		m_Position = glm::vec3(0.0f);
		m_Texture = nullptr;
	}

	void TerrainChunk::CreateChunk(const std::array<int, ChunkCellCount>& chunkCellData)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<VertexIndex> indices = std::vector<VertexIndex>();

		const float TriWidth = WorldChunkSize / ChunkCellWidth;
		const VertexIndex RestartValue = IndexBuffer::GetPrimitiveEndIndex();

		glm::vec3 position = glm::vec3(0.0f);
		printf("%f %f %f\n", m_Position.x, m_Position.y, m_Position.z);

		siv::PerlinNoise pn(800012);
		float noiseSizeScale = 2.0f;
		float noiseDistanceScale = 1 / 16.0f;

		glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

		//Calculating Vertices
		for (unsigned int z = 0; z < ChunkCellWidth; z++)
		{
			for (unsigned int x = 0; x <= ChunkCellWidth; x++)
			{
				vertices.push_back(Vertex(glm::vec3(
					position.x + x * TriWidth,
					position.y + (noiseSizeScale * pn.noise2D((m_Position.x + x * TriWidth) * noiseDistanceScale, (m_Position.z + z * TriWidth) * noiseDistanceScale)),
					position.z + z * TriWidth), normal));

				vertices.push_back(Vertex(glm::vec3(
					position.x + x * TriWidth,
					position.y + (noiseSizeScale * pn.noise2D((m_Position.x + x * TriWidth) * noiseDistanceScale, (m_Position.z + (z + 1) * TriWidth) * noiseDistanceScale)),
					position.z + (z + 1) * TriWidth), normal));
			}
		}

		const int ExpectedIndexCountPerRow = (ChunkCellWidth * 2) + 2;

		for (int r = 0; r < ChunkCellWidth; r++)
		{
			for (int i = 0; i < ExpectedIndexCountPerRow; i++)
			{
				indices.push_back((r * ExpectedIndexCountPerRow) + i);
			}

			indices.push_back(RestartValue);
		}

		VertexBuffer::Create(m_VertexBuffer, vertices);
		IndexBuffer::Create(m_IndexBuffer, indices);
	}

	void TerrainChunk::Update(const float& deltaTime)
	{

	}

	void TerrainChunk::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, glm::vec3(1.0f));
		glm::mat4 translate = glm::translate(matrix, m_Position);
		matrix = translate * scale;
		renderer.PushConstants.ModelMatrix = matrix;
		renderer.UpdatePushConstants();

		renderer.PushDescriptorSet(m_Texture);

		if (m_IndexBuffer.Exists())
		{
			m_IndexBuffer.Bind();
		}
		
		if (m_VertexBuffer.Exists())
		{
			m_VertexBuffer.Bind();
		}

		if (m_IndexBuffer.Exists() && m_VertexBuffer.Exists())
		{
			renderer.SetCustomGraphicsPipeline(m_TerrainPipeline);
			vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), 1, 0, 0, 0);
			renderer.SetSolidGraphicsPipeline();
		}
	}
}