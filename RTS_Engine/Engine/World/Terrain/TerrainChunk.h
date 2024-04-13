#pragma once
#include <Rendering/VertexBuffer.h>
#include <Rendering/IndexBuffer.h>
#include <Rendering/CustomPipelineObject.h>

namespace Bennett
{
	class Renderer;
	class Texture;

	constexpr int ChunkCellWidth = 8;
	constexpr int ChunkCellCount = (ChunkCellWidth * ChunkCellWidth);

	class BENNETT_ENGINE TerrainChunk
	{
	private:
		CustomPipeline m_TerrainPipeline;

		glm::vec3 m_Position;
		Texture* m_Texture;
		IndexBuffer m_IndexBuffer;
		VertexBuffer m_VertexBuffer;

	public:
		TerrainChunk(const glm::vec3& position);
		~TerrainChunk();

		void CreateChunk(const std::array<int, ChunkCellCount>& chunkCellData);

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer);
	};
}