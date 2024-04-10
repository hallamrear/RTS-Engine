#include <BennettPCH.h>
#include <Defines/MarchingCubesTriangulationTable.h>
#include <System/Manager/AssetManager.h>
#include <System/ServiceLocator.h>
#include <World/Terrain/Terrain.h>
#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>
#include <Rendering/ShaderLoader.h>
#include <cmath>

namespace Bennett
{
	Terrain::Terrain() : Entity("Terrain")
	{
		m_Texture = nullptr;
		m_Indices = std::vector<VertexIndex>();
		m_Vertices = std::vector<Vertex>();
		m_VertexBuffer = {};
		m_IndexBuffer = {};
		m_TerrainPipeline = {};

	}

	Terrain::~Terrain()
	{
		Destroy();
	}

	void Terrain::Generate(const std::string& terrainFile)
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		m_Texture = AssetManager::GetTexture("red");










	}

	void Terrain::Destroy()
	{
		m_Texture = nullptr;

		VertexBuffer::Destroy(m_VertexBuffer);
		IndexBuffer::Destroy(m_IndexBuffer);
	}

	void Terrain::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		renderer.PushConstants.ModelMatrix = GetTransform().GetModelMatrix();
		renderer.UpdatePushConstants();

		renderer.PushDescriptorSet(m_Texture);
 		renderer.UpdateUniformBuffers();
		
		if(m_VertexBuffer.Exists())
			m_VertexBuffer.Bind();

		if(m_IndexBuffer.Exists())
			m_IndexBuffer.Bind();

		vkCmdDrawIndexed(renderer.GetCommandBuffer(), m_IndexBuffer.Count(), TERRAIN_CHUNK_COUNT, 0, 0, 0);
	}
}