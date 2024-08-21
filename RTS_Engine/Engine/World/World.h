#pragma once
#include <vector>
#include <unordered_map>
#include <Physics/PhysicsWorld.h>

namespace Bennett
{
	class Renderer;
	class Terrain;
	class WorldChunk;
	class BEntity;
	class BActor;
	class BProp;

	class BENNETT_ENGINE World
	{
	private:
		friend class LevelManager;

		PhysicsWorld m_PhysicsWorld;

		bool m_IsLoaded;
		std::unordered_map<std::string, BEntity*>	m_Entities;
		std::unordered_map<std::string, BActor*>	m_Actors;
		std::unordered_map<std::string, BProp*>		m_Props;
		std::unordered_map<glm::ivec2, WorldChunk*> m_SpatialGrid;

	protected:
		void DeleteAllEntities();
		void AddEntityToSpatialGrid(BEntity& entity);
		WorldChunk* CreateChunk(const glm::ivec2& id);
		void RemoveEntityFromSpatialGrid(BEntity& entity);

		glm::ivec2 GetChunkIDOfPosition(const glm::vec3& position) const;

	public:
		World();
		~World();

		WorldChunk* GetWorldChunk(const glm::vec3& position) const;
		WorldChunk* GetWorldChunk(const glm::ivec2& id) const;
		BActor* SpawnActor(const std::string& name, const Transform& transform);
		void GetAllActors(std::vector<BActor*>& actors);
		BActor* GetActor(const std::string& name);
		BProp* SpawnProp(const std::string& name, const Transform& transform);
		BProp* GetProp(const std::string& name);
		void DestroyEntity(BEntity* entity);

		void PreloadChunks(const std::vector<glm::ivec2>& chunkIds);

		float GetTerrainHeight(const glm::vec3& position) const;

		const bool IsLoaded() const;

		void Update(const float& DeltaTime);
		void Render(const Renderer& renderer);
	};
}
