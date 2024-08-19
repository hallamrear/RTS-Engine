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

		std::vector<BEntity*> m_Entities;
		WorldChunk(const glm::ivec2& id);

	protected:
		static WorldChunk* Create(const glm::ivec2& id);
		void GenerateTerrainChunk();

	public:
		~WorldChunk();

		std::vector<BEntity*>& GetAllEntities();
		void RemoveAllEntities();
		void AddEntity(BEntity& entity);
		void RemoveEntity(BEntity& entity);

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer) const;
	};
}

