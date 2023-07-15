#pragma once
#include <unordered_map>

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

		Entity* CreateTerrain();

		Entity* SpawnEntity(const std::string& name);
		void DestroyEntity(Entity* entity);

		const bool IsLoaded() const;

		void Update(const float& DeltaTime);
		void Render(const Renderer& renderer);
	};
}
