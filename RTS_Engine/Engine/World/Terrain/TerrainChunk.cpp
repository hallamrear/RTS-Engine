#include "BennettPCH.h"
#include "TerrainChunk.h"
#include <Rendering/Renderer.h>
#include <System/ServiceLocator.h>
#include <System/Assets/AssetManager.h>
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

		for (size_t i = 0; i < ChunkCellCount; i++)
		{
			data[i] = rand() % 3;
		}

		SetChunkData(data);
		CreateChunk();

		CustomPipelineDetails details;
		details.PrimitiveRestartEnabled = VK_FALSE;
		details.Topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		details.PolygonMode = VK_POLYGON_MODE_LINE;

		ServiceLocator::GetRenderer().CreateCustomPipeline(m_TerrainPipeline, details);
	}

	TerrainChunk::~TerrainChunk()
	{
		m_Position = glm::vec3(0.0f);
		m_Texture = nullptr;

		if (m_IndexBuffer.Exists())
		{
			IndexBuffer::Destroy(m_IndexBuffer);
		}

		if (m_VertexBuffer.Exists())
		{
			VertexBuffer::Destroy(m_VertexBuffer);
		}
	}

	const glm::vec3& TerrainChunk::GetChunkCornerPosition() const
	{
		return m_Position;
	}

	const std::array<int, ChunkCellCount>& TerrainChunk::GetChunkData() const
	{
		return m_ChunkData;
	}

	void TerrainChunk::SetChunkData(const std::array<int, ChunkCellCount>& chunkCellData)
	{
		m_ChunkData = chunkCellData;
	}

	void TerrainChunk::CreateChunk()
	{
		if (m_ChunkData.size() > 0)
		{
			CreateChunk(m_ChunkData);
		}
		else
		{
			Log(LOG_MINIMAL, "Tried to create chunk without any data.\n");
		}
	}

	float TerrainChunk::GetHeightAtPointInCell(const glm::ivec2& cellLocationInChunk) const
	{
		int HeightDataIndex = (ChunkCellWidth * cellLocationInChunk.y) + cellLocationInChunk.x;

		if (HeightDataIndex > m_ChunkData.size())
			return 0.0f;

		return (m_ChunkData[HeightDataIndex] * ChunkCellHeightStep);
	}

	void TerrainChunk::CreateChunk(const std::array<int, ChunkCellCount>& chunkCellData)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<VertexIndex> indices = std::vector<VertexIndex>();

		const float TriWidth = WorldChunkSize / ChunkCellWidth;
		const VertexIndex RestartValue = IndexBuffer::GetPrimitiveEndIndex();

		glm::vec3 position = glm::vec3(0.0f);
		siv::PerlinNoise pn(800012);
		float noiseSizeScale = 2.0f;
		float noiseDistanceScale = 1 / 16.0f;

		glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

		float height = 0.0f;
		uint index = -1;

		//Calculating Vertices and Indices

		for (unsigned int i = 0; i < ChunkCellCount; i++)
		{
			index = i;
			uint x = i % ChunkCellWidth;
			uint z = i / ChunkCellWidth;

			if (chunkCellData[index] < 100)
			{
				height = chunkCellData[index] * ChunkCellHeightStep;

				VertexIndex indexCount = (VertexIndex)vertices.size();

				//Push Flat
				vertices.push_back(Vertex(glm::vec3(
					position.x + x * TriWidth,
					position.y + height,
					position.z + z * TriWidth), normal));

				vertices.push_back(Vertex(glm::vec3(
					position.x + x * TriWidth,
					position.y + height,
					position.z + (z + 1) * TriWidth), normal));

				vertices.push_back(Vertex(glm::vec3(
					position.x + (x + 1) * TriWidth,
					position.y + height,
					position.z + (z + 1) * TriWidth), normal));

				vertices.push_back(Vertex(glm::vec3(
					position.x + (x + 1) * TriWidth,
					position.y + height,
					position.z + z * TriWidth), normal));

				indices.push_back(indexCount + 0);
				indices.push_back(indexCount + 1);
				indices.push_back(indexCount + 2);
				indices.push_back(indexCount + 0);
				indices.push_back(indexCount + 2);
				indices.push_back(indexCount + 3);
			}
			else
			{
				//Push Ramp
			}
		}

		/*const int ExpectedIndexCountPerRow = (ChunkCellWidth * 2) + 2;

		for (int r = 0; r < ChunkCellWidth; r++)
		{
			for (int i = 0; i < ExpectedIndexCountPerRow; i++)
			{
				indices.push_back((r * ExpectedIndexCountPerRow) + i);
			}

			indices.push_back(RestartValue);
		}*/

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