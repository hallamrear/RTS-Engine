#pragma once
#include <World/Entity.h>
#include <vector>
#include <Rendering/IndexBuffer.h>
#include <Rendering/VertexBuffer.h>
#include <World/Terrain/TerrainVertex.h>
#include <Rendering/CustomPipelineObject.h>

namespace Bennett
{
	/// <summary>
	/// Specification:
	/// 
	/// Custom Graphics Pipeline:
	/// - Triangle Strip
	/// - Using custom vertex.
	/// 
	/// </summary>
	/// 
	class TerrainChunk;
	class Texture;

	class BENNETT_ENGINE Terrain : public Entity
	{
	private:
		CustomPipeline m_TerrainPipeline;
		IndexBuffer m_IndexBuffer;
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
