#pragma once
#include "Entity.h"

namespace Bennett
{
	class TerrainChunk : public Entity
	{
	private:
		static int s_TerrainIDCount;
	public:

		TerrainChunk();
		~TerrainChunk();

		void BuildChunk();
	};
}