#pragma once
#include <World/Entity.h>
#include <vector>
#include <Rendering/VertexBuffer.h>
#include <World/Terrain/TerrainVertex.h>
#include <Rendering/CustomPipelineObject.h>

#define TERRAIN_CELL_SIZE 8.0f
#define TERRAIN_WIDTH 64
#define TERRAIN_CHUNK_COUNT (TERRAIN_WIDTH * TERRAIN_WIDTH)

namespace Bennett
{
	class TerrainChunk;
	class Texture;
	class TerrainDetailBuffer;

	class BENNETT_ENGINE Terrain : public Entity
	{
	private:
		glm::vec2 m_ChunkLocations[TERRAIN_CHUNK_COUNT];
		CustomPipeline m_TerrainPipeline;
		VertexBuffer m_VertexBuffer;
		Texture* m_Texture;

		std::vector<VertexIndex> m_Indices;
		std::vector<TerrainVertex> m_Vertices;

		Terrain(int size);
		void Generate();
		void CreateTerrainChunkMesh();

	public:
		~Terrain();

		static Terrain* Create(int size);

		void Render(const Renderer& renderer) override;
	};
};
