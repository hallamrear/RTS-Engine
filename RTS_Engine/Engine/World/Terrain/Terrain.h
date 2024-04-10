#pragma once
#include <World/Entity.h>
#include <vector>
#include <Rendering/VertexBuffer.h>
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
		std::vector<Vertex> m_Vertices;


	public:
		Terrain();
		~Terrain();

		virtual void Generate(const std::string& terrainFile);

		void Destroy();

		virtual void Render(const Renderer& renderer) override;
	};
};
