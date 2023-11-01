#pragma once
#include <World/Entity.h>

namespace Bennett
{
	class TerrainChunk;

	class BENNETT_ENGINE Terrain : public Entity
	{
	private:
		int m_ChunkCountXZ;
		std::vector<TerrainChunk*> m_Chunks;

		Terrain(int size);
		void Generate();

	public:
		~Terrain();

		static Terrain* Create(int size);

		void Render(const Renderer& renderer) override;
	};
};
