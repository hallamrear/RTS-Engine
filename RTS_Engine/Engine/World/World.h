#pragma once
#include <unordered_map>
#include <Collision/Octree.h>

namespace Bennett
{
	class Renderer;
	class Entity;
	class Terrain;
	class WorldChunk;

	class BENNETT_ENGINE World
	{
	private:
		friend class LevelManager;

		bool m_IsLoaded;
		std::unordered_map<std::string, Entity*> m_Entities;
		std::unordered_map<glm::ivec2, WorldChunk*> m_SpatialGrid;

	protected:
		void DeleteAllEntities();
		void AddEntityToSpatialGrid(const Entity& entity);
		WorldChunk* CreateChunk(const glm::ivec2& id);
		void RemoveEntityFromSpatialGrid(const Entity& entity);

		glm::ivec2 GetChunkIDOfPosition(const glm::vec3& position) const;

	public:
		World();
		~World();

		WorldChunk* GetWorldChunk(const glm::ivec2& id) const;
		Entity* SpawnEntity(const std::string& name);
		Entity* SpawnEntity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation);
		Entity* SpawnTESTEntity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation);
		Entity* GetEntity(const std::string& name);
		void DestroyEntity(Entity* entity);

		void PreloadChunks(const std::vector<glm::ivec2>& chunkIds);

		float GetTerrainHeight(const glm::vec3& position) const;

		const bool IsLoaded() const;

		void Update(const float& DeltaTime);
		void Render(const Renderer& renderer);
	};
}
