#include <BennettPCH.h>
#include <World/MoveableTestEntity.h>
#include <System/InputMonitor.h>

namespace Bennett
{
	MoveableTestEntity::MoveableTestEntity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation)
		: Entity(name, scale, position, rotation)
	{
		//Creating a list of keys I want to track.
		std::vector<int> keys =
		{
			BENNETT_KEY_I,
			BENNETT_KEY_K,
			BENNETT_KEY_J,
			BENNETT_KEY_L,
			BENNETT_KEY_O,
			BENNETT_KEY_U,
		};

		//Creating the object that tracks keypresses and giving it the list.
		inputMonitor = new InputMonitor(keys);
	}

	MoveableTestEntity::~MoveableTestEntity()
	{
		if (inputMonitor)
		{
			delete inputMonitor;
			inputMonitor = nullptr;
		}
	}

	void MoveableTestEntity::Update(const float& deltaTime)
	{
		float movespeed = 5.0f;

		if (inputMonitor->GetKeyState(BENNETT_KEY_I))
		{
			Translate(GetForwardVector() * movespeed * deltaTime);
		}

		if (inputMonitor->GetKeyState(BENNETT_KEY_K))
		{
			Translate(-GetForwardVector() * movespeed * deltaTime);
		}

		if (inputMonitor->GetKeyState(BENNETT_KEY_J))
		{
			Translate(-GetRightVector() * movespeed * deltaTime);
		}

		if (inputMonitor->GetKeyState(BENNETT_KEY_L))
		{
			Translate(GetRightVector() * movespeed * deltaTime);
		}

		if (inputMonitor->GetKeyState(BENNETT_KEY_O))
		{
			Translate(GetUpVector() * movespeed * deltaTime);
		}

		if (inputMonitor->GetKeyState(BENNETT_KEY_U))
		{
			Translate(-GetUpVector() * movespeed * deltaTime);
		}

		Entity::Update(deltaTime);
	}

	void MoveableTestEntity::Render(const Renderer& renderer)
	{
		Entity::Render(renderer);
	}
}