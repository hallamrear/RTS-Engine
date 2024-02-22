#include <BennettPCH.h>
#include <World/MoveableTestEntity.h>
#include <System/InputMonitor.h>
#include <Collision/Collider/Collider.h>

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
			BENNETT_KEY_M,
			BENNETT_KEY_N,
		};

		//Creating the object that tracks keypresses and giving it the list.
		m_InputMonitor = new InputMonitor(keys);
	}

	MoveableTestEntity::~MoveableTestEntity()
	{
		if (m_InputMonitor)
		{
			delete m_InputMonitor;
			m_InputMonitor = nullptr;
		}
	}

	void MoveableTestEntity::Update(const float& deltaTime)
	{
		float movespeed = 5.0f;

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_I))
		{
			GetTransform().Translate(GetTransform().GetForwardVector() * movespeed * deltaTime);
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_K))
		{
			GetTransform().Translate(-GetTransform().GetForwardVector() * movespeed * deltaTime);
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_J))
		{
			GetTransform().Translate(-GetTransform().GetRightVector() * movespeed * deltaTime);
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_L))
		{
			GetTransform().Translate(GetTransform().GetRightVector() * movespeed * deltaTime);
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_U))
		{
			GetTransform().Rotate(glm::vec3(0.0f, -10 * movespeed * deltaTime, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_O))
		{
			GetTransform().Rotate(glm::vec3(0.0f, +10 * movespeed * deltaTime, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_M))
		{
			GetTransform().Translate(GetTransform().GetUpVector() * movespeed * deltaTime);
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_N))
		{
			GetTransform().Translate(-GetTransform().GetUpVector() * movespeed * deltaTime);
		}

		Entity::Update(deltaTime);
	}

	void MoveableTestEntity::Render(const Renderer& renderer)
	{
		Entity::Render(renderer);
	}
}