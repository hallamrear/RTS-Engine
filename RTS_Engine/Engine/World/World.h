#pragma once
#include <unordered_map>
#include <Physics/Collision/Octree.h>

namespace Bennett
{
	class Renderer;
	class BEntity;
	class Terrain;
	class WorldChunk;
	class BActor;

	class BENNETT_ENGINE World
	{
	private:
		friend class LevelManager;

		bool m_IsLoaded;
		std::unordered_map<std::string, BEntity*> m_Entities;
		std::unordered_map<glm::ivec2, WorldChunk*> m_SpatialGrid;

	protected:
		void DeleteAllEntities();
		void AddEntityToSpatialGrid(const BEntity& entity);
		WorldChunk* CreateChunk(const glm::ivec2& id);
		void RemoveEntityFromSpatialGrid(const BEntity& entity);

		glm::ivec2 GetChunkIDOfPosition(const glm::vec3& position) const;

	public:
		World();
		~World();

		WorldChunk* GetWorldChunk(const glm::vec3& position) const;
		WorldChunk* GetWorldChunk(const glm::ivec2& id) const;
		BActor* SpawnActor(const std::string& name, const Transform& transform);
		BActor* GetEntity(const std::string& name);
		void DestroyEntity(BEntity* entity);

		void PreloadChunks(const std::vector<glm::ivec2>& chunkIds);

		float GetTerrainHeight(const glm::vec3& position) const;

		const bool IsLoaded() const;

		void Update(const float& DeltaTime);
		void Render(const Renderer& renderer);
	};
}
