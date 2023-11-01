#include <BennettPCH.h>
#include <Defines/MarchingCubesTriangulationTable.h>
#include <System/Manager/AssetManager.h>
#include <System/ServiceLocator.h>
#include <World/Terrain.h>
#include <World/TerrainChunk.h>

namespace Bennett
{
	Terrain::Terrain(int size) : Entity("Terrain")
	{
		m_ChunkCountXZ = size;
	}

	Terrain::~Terrain()
	{
		m_ChunkCountXZ = -1;
	}

	void Terrain::Generate()
	{
		const float offset = TriangulationData::CELL_SIZE / TriangulationData::GRID_RESOLUTION;

		for (int i = 0; i < m_ChunkCountXZ; i++)
		{
			for (int k = 0; k < m_ChunkCountXZ; k++)
			{
				TerrainChunk* chunk = new TerrainChunk();
				m_Chunks.push_back(chunk);
				m_Chunks.back()->SetPosition(
					glm::vec3(
						i * offset,
						0.0f,
						k * offset));
				m_Chunks.back()->BuildChunk();
				m_Chunks.back()->SetTexture(ServiceLocator::GetAssetManager().GetTexture("Floor"));
			}
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
		size_t chunkCount = m_Chunks.size();
		for (size_t i = 0; i < chunkCount; i++)
		{
			m_Chunks[i]->Render(renderer);
		}
	}
}