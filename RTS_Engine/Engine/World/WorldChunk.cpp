#include "BennettPCH.h"
#include <Rendering/Renderer.h>
#include <System/ServiceLocator.h>
#include <System/Assets/AssetManager.h>
#include <World/Terrain/TerrainChunk.h>
#include <World/WorldChunk.h>
#include <World/Entity/Entity.h>

namespace Bennett
{
	WorldChunk* WorldChunk::Create(const glm::ivec2& id)
	{
		WorldChunk* chunk = new WorldChunk(id);
		chunk->GenerateTerrainChunk();
		return chunk;
	}

	void WorldChunk::GenerateTerrainChunk()
	{
		m_TerrainChunk = new TerrainChunk(glm::vec3(m_ID.x * WorldChunkSize, 0.0f, m_ID.y * WorldChunkSize));
	}

	WorldChunk::WorldChunk(const glm::ivec2& id)
	{
		m_Entities = std::vector<const Entity*>();
		m_ID = id;
		m_TerrainChunk = nullptr;
		m_Model = ServiceLocator::GetAssetManager().GetAsset<Model>("1x1_Cube");
	}

	WorldChunk::~WorldChunk()
	{
		m_Entities.clear();
		m_Model = nullptr;

		if (m_TerrainChunk != nullptr)
		{
			delete m_TerrainChunk;
			m_TerrainChunk = nullptr;
		}
	}

	std::vector<const Entity*>& WorldChunk::GetAllEntities()
	{
		return m_Entities;
	}

	void WorldChunk::RemoveAllEntities()
	{
		m_Entities.clear();
	}

	void WorldChunk::AddEntity(const Entity& entity)
	{
		m_Entities.push_back(&entity);
	}

	void WorldChunk::RemoveEntity(const Entity& entity)
	{
		auto itr = std::find(m_Entities.begin(), m_Entities.end(), &entity);

		//Found entity in list.
		if (itr != m_Entities.end())
		{
			m_Entities.erase(itr);
		}
		else
		{
			Log(LOG_MINIMAL, "Failed to find \"%s\" in grid cell %i,%i and so has not be safely removed.\n", entity.GetName(), m_ID.x, m_ID.y);
		}
	}

	void WorldChunk::Update(const float& deltaTime)
	{

	}

	void WorldChunk::Render(const Renderer& renderer) const
	{
		glm::vec3 chunkCentre = glm::vec3((m_ID.x * WorldChunkSize) + (WorldChunkSize / 2), 0.0f, (m_ID.y * WorldChunkSize) + (WorldChunkSize / 2));

		if (ENABLE_DRAW_WORLD_CHUNK_OUTLINE)
		{
			//Rendering centre cube.
			glm::mat4 matrix = glm::mat4(1.0f);
			glm::mat4 scale = glm::scale(matrix, glm::vec3(1.0f));
			glm::mat4 translate = glm::translate(matrix, chunkCentre);
			matrix = translate * scale;
			renderer.PushConstants.ModelMatrix = matrix;
			renderer.PushDescriptorSet(&renderer.GetDebugTexture());
			renderer.UpdatePushConstants();
			m_Model->Render(renderer);

			const CustomPipeline& existingPipeline = *renderer.GetCurrentGraphicsPipeline();
			renderer.SetWireframeGraphicsPipeline();
			matrix = glm::mat4(1.0f);
			scale = glm::scale(matrix, glm::vec3(WorldChunkSize, 200.0f, WorldChunkSize));
			translate = glm::translate(matrix, chunkCentre);
			matrix = translate * scale;
			renderer.PushConstants.ModelMatrix = matrix;
			renderer.PushDescriptorSet(&renderer.GetDebugTexture());
			renderer.UpdatePushConstants();
			m_Model->Render(renderer);
			renderer.SetCustomGraphicsPipeline(existingPipeline);
		}

		if (m_TerrainChunk != nullptr)
		{
			m_TerrainChunk->Render(renderer);
		}
	}
}