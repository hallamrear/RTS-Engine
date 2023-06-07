#pragma once
#include "World.h"

namespace Bennett
{
	class Renderer;

	class LevelManager
	{
	private:

	public:
		static void LoadLevel(const Renderer& renderer, const std::string& levelName, World& world);
		static void UnloadLevel(World& world);
	};
}