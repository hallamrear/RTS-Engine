#pragma once
#include <World/Entity.h>
#include <vector>
#include <Rendering/VertexBuffer.h>
#include <World/Terrain/TerrainVertex.h>
#include <Rendering/CustomPipelineObject.h>
#include <Defines/TerrainDefines.h>

namespace Bennett
{
	class TerrainChunk;
	class Texture;
	class TerrainDetailBuffer;

	class BENNETT_ENGINE Terrain : public Entity
	{
	private:
		CustomPipeline m_TerrainPipeline;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		Texture* m_Texture;

		std::vector<VertexIndex> m_Indices;
		std::vector<TerrainVertex> m_Vertices;

		Terrain();
		virtual void Generate(const long& seed);
		virtual double GetPointHeight(const glm::vec3& position);
		void CreateTerrainChunkMesh();

	public:
		ChunkPosition m_ChunkLocations[TERRAIN_CHUNK_COUNT];

		~Terrain();

		static Terrain* Create(const long& seed);

		virtual void Render(const Renderer& renderer) override;
	};
};
