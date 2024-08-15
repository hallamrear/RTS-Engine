#include <BennettPCH.h>
#include <World/Entity/MoveableTestEntity.h>
#include <System/InputMonitor.h>
#include <Physics/Collision/Collider/Collider.h>
#include <Rendering/Renderer.h>

namespace Bennett
{
	MoveableTestEntity::MoveableTestEntity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation)
		: Entity(name, scale, position, rotation)
	{
		m_MovementEnabled = true;

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
			BENNETT_KEY_Y,
			BENNETT_KEY_H,
		};

		//Creating the object that tracks keypresses and giving it the list.
		m_InputMonitor = new InputMonitor(keys);

		m_Rigidbody = new Rigidbody(GetTransform());
	}

	MoveableTestEntity::~MoveableTestEntity()
	{
		delete m_Rigidbody;
		m_Rigidbody = nullptr;

		if (m_InputMonitor)
		{
			delete m_InputMonitor;
			m_InputMonitor = nullptr;
		}
	}

	Rigidbody* MoveableTestEntity::GetRigidbody() const
	{
		if (m_Rigidbody)
			return m_Rigidbody;

		return nullptr;
	}

	const bool& MoveableTestEntity::IsMovementEnabled() const
	{
		return m_MovementEnabled;
	}

	void MoveableTestEntity::SetMovementEnabled(const bool& movementEnabled)
	{
		m_MovementEnabled = movementEnabled;
	}

	void MoveableTestEntity::Update(const float& deltaTime)
	{
		float movespeed = 5.0f;

		if (m_MovementEnabled)
		{
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
				GetTransform().Translate(GetTransform().GetLeftVector() * movespeed * deltaTime);
			}

			if (m_InputMonitor->GetKeyState(BENNETT_KEY_L))
			{
				GetTransform().Translate(-GetTransform().GetLeftVector() * movespeed * deltaTime);
			}

			if (m_InputMonitor->GetKeyState(BENNETT_KEY_U))
			{
				GetTransform().Rotate(glm::vec3(0.0f, +10 * movespeed * deltaTime, 0.0f));
			}

			if (m_InputMonitor->GetKeyState(BENNETT_KEY_O))
			{
				GetTransform().Rotate(glm::vec3(0.0f, -10 * movespeed * deltaTime, 0.0f));
			}

			if (m_InputMonitor->GetKeyState(BENNETT_KEY_Y))
			{
				GetTransform().Rotate(glm::vec3(-10 * movespeed * deltaTime, 0.0f, 0.0f));
			}

			if (m_InputMonitor->GetKeyState(BENNETT_KEY_H))
			{
				GetTransform().Rotate(glm::vec3(+10 * movespeed * deltaTime, 0.0f, 0.0f));
			}

			if (m_InputMonitor->GetKeyState(BENNETT_KEY_M))
			{
				GetTransform().Translate(GetTransform().GetUpVector() * movespeed * deltaTime);
			}

			if (m_InputMonitor->GetKeyState(BENNETT_KEY_N))
			{
				GetTransform().Translate(-GetTransform().GetUpVector() * movespeed * deltaTime);
			}
		}

		//m_Rigidbody->Update(deltaTime);
		Entity::Update(deltaTime);
	}

	void MoveableTestEntity::Render(const Renderer& renderer)
	{
		renderer.DrawDebugLine(GetTransform().GetPosition(), GetTransform().GetForwardVector(), 5.0f);
		renderer.DrawDebugLine(GetTransform().GetPosition(), GetTransform().GetLeftVector(), 5.0f);
		renderer.DrawDebugLine(GetTransform().GetPosition(), GetTransform().GetUpVector(), 5.0f);

		Entity::Render(renderer);
	}
}