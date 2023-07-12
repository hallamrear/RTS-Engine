#pragma once
#include "Entity.h"

namespace Bennett
{
	class TerrainChunk : public Entity
	{
	private:
		static int s_TerrainIDCount;
		static const int CHUNK_SIZE = 32;
		static const int HALF_SIZE = CHUNK_SIZE / 2;
		static const int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
		static const int CHUNK_VOLUME = CHUNK_AREA * CHUNK_SIZE;

		static inline const int GetIndexFromPosition(const glm::vec3& position)
		{
			return position.x + CHUNK_SIZE * position.z + CHUNK_AREA * position.y;
		};

	public:

		TerrainChunk();
		~TerrainChunk();

		void BuildChunk();
	};
}