#pragma once
#include <System/Engine.h>
#include "resource.h"

class Game : public Bennett::Engine
{
private:
	Bennett::Window* m_Window;
	std::vector<class Bennett::BActor*> m_SelectedEntities;

public:
	Game();
	~Game();
	bool Initialise();

	void RunGameLoop();
};