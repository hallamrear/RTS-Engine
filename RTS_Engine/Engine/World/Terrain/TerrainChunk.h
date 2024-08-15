#pragma once
#include <Rendering/VertexBuffer.h>
#include <Rendering/IndexBuffer.h>
#include <Rendering/CustomPipelineObject.h>

namespace Bennett
{
	class Renderer;
	class Texture;

	constexpr int ChunkCellWidth = 16;
	constexpr int ChunkCellCount = (ChunkCellWidth * ChunkCellWidth);
	constexpr float ChunkCellHeightStep = 0.0f;

	class BENNETT_ENGINE TerrainChunk
	{
	private:
		Texture* m_Texture;
		glm::vec3 m_Position;
		IndexBuffer m_IndexBuffer;
		VertexBuffer m_VertexBuffer;
		CustomPipeline m_TerrainPipeline;
		std::array<int, ChunkCellCount> m_ChunkData;
		void CreateChunk(const std::array<int, ChunkCellCount>& chunkCellData);

	public:
		TerrainChunk(const glm::vec3& position);
		~TerrainChunk();

		const glm::vec3& GetChunkCornerPosition() const;

		const std::array<int, ChunkCellCount>& GetChunkData() const;
		void SetChunkData(const std::array<int, ChunkCellCount>& chunkCellData);
		void CreateChunk();

		float GetHeightAtPointInCell(const glm::ivec2& cellLocationInChunk) const;

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer);
	};
}