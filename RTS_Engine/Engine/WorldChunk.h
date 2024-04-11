#pragma once

namespace Bennett
{
	constexpr int WorldChunkSize = 32;

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
		WorldChunk();

	protected:
		static WorldChunk* Create(const glm::ivec2& id);

	public:
		~WorldChunk();

		void AddEntity(const Entity& entity);
		void RemoveEntity(const Entity& entity);

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer) const;
	};
}

