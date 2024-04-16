#pragma once

namespace Bennett
{
	constexpr int WorldChunkSize = 16;

	class TerrainChunk;
	class Renderer;
	class Entity;

	class BENNETT_ENGINE WorldChunk
	{
	private:
		Model* m_Model;

		friend class World;

		glm::ivec2 m_ID;
		TerrainChunk* m_TerrainChunk;

		std::vector<const Entity*> m_Entities;
		WorldChunk(const glm::ivec2& id);

	protected:
		static WorldChunk* Create(const glm::ivec2& id);
		void GenerateTerrainChunk();

	public:
		~WorldChunk();

		void RemoveAllEntities();
		void AddEntity(const Entity& entity);
		void RemoveEntity(const Entity& entity);

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer) const;
	};
}

