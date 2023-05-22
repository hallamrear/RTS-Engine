#pragma once
#include <unordered_map>

namespace Bennett
{
	class Renderer;
	class Entity;

	class World
	{
	private:
		bool m_IsLoaded;
		std::unordered_map<std::string, Entity*> m_Entities;

	protected:
		void DeleteAllEntities();

	public:
		World();
		~World();


		Entity* SpawnEntity(const std::string& name);
		void DestroyEntity(Entity* entity);

		const bool IsLoaded() const;

		void Update(const float& DeltaTime);
		void Render(const Renderer& renderer);
	};
}
