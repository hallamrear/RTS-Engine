#pragma once
#include "Entity.h"

namespace Bennett
{
	class Terrain : public Entity
	{
	private:
		static int s_TerrainIDCount;

		Terrain(int size);
		void Generate();

	public:
		~Terrain();


		Entity* Create(int size);
	};
};
