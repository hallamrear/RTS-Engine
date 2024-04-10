#pragma once
#include <unordered_map>
#include <Collision/Octree.h>

namespace Bennett
{
	class Renderer;
	class Entity;
	class Terrain;

	class BENNETT_ENGINE World
	{
	private:
		friend class LevelManager;

		bool m_IsLoaded;
		std::unordered_map<std::string, Entity*> m_Entities;

	protected:
		void DeleteAllEntities();

	public:
		World();
		~World();

		Terrain* CreateTerrain();

		Entity* SpawnTestEntity(const std::string& name);
		Entity* SpawnEntity(const std::string& name);
		Entity* SpawnEntity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation);
		Entity* GetEntity(const std::string& name);
		void DestroyEntity(Entity* entity);

		const bool IsLoaded() const;

		void Update(const float& DeltaTime);
		void Render(const Renderer& renderer);
	};
}
