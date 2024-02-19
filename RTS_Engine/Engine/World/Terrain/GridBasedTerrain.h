#pragma once
#include <World/Entity.h>
#include <Rendering/CustomPipelineObject.h>
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"

namespace Bennett
{
	class BENNETT_ENGINE GridBasedTerrain : public Entity
	{
	private:
		int gridTypes[128][128] = {};

		CustomPipeline m_TerrainPipeline;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		Texture* m_Texture;
		GridBasedTerrain();

		void LoadTerrainDataFromFile(const std::string& filePath);
		void GenerateMesh(const std::string& filePath);

	public:
		~GridBasedTerrain();

		static GridBasedTerrain* Create(const std::string& filePath);

		void Render(const Renderer& renderer) override;
	};
}