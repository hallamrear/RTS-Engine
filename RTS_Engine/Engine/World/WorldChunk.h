#pragma once

namespace Bennett
{
	constexpr int WorldChunkSize = 32;

	class TerrainChunk;
	class Renderer;
	class BEntity;

	class BENNETT_ENGINE WorldChunk
	{
	private:
		Model* m_Model;

		friend class World;

		glm::ivec2 m_ID;
		TerrainChunk* m_TerrainChunk;

		std::vector<const BEntity*> m_Entities;
		WorldChunk(const glm::ivec2& id);

	protected:
		static WorldChunk* Create(const glm::ivec2& id);
		void GenerateTerrainChunk();

	public:
		~WorldChunk();

		std::vector<const BEntity*>& GetAllEntities();
		void RemoveAllEntities();
		void AddEntity(const BEntity& entity);
		void RemoveEntity(const BEntity& entity);

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer) const;
	};
}

