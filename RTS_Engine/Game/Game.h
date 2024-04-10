#pragma once
#include <System/Engine.h>
#include "resource.h"

class Game : public Bennett::Engine
{
private:
	Bennett::Window* m_Window;

public:
	Game();
	~Game();
	bool Initialise();

	void InitTestTerrainScene();
	void InitTestEntitiesScene();
	void InitTestOctreeScene();

	void RunGameLoop();
};