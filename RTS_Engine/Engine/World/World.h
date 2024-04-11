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
		std::unordered_map<glm::vec2, WorldChunk*> m_SpatialGrid;

	protected:
		void DeleteAllEntities();
		void AddEntityToSpatialGrid(const Entity& entity);
		void RemoveEntityFromSpatialGrid(const Entity& entity);

		glm::ivec2 GetChunkIDOfPosition(const glm::vec3& position);

	public:
		World();
		~World();

		Entity* SpawnEntity(const std::string& name);
		Entity* SpawnEntity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation);
		Entity* GetEntity(const std::string& name);
		void DestroyEntity(Entity* entity);

		const bool IsLoaded() const;

		void Update(const float& DeltaTime);
		void Render(const Renderer& renderer);
	};
}
