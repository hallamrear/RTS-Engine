#include "BennettPCH.h"
#include "BActor.h"
#include <Defines/HelperFunctions.h>

namespace Bennett
{
	BActor::BActor(const std::string& name, const Transform& transform) : BProp(name, transform), m_SelectionCollider(GetTransform())
	{
		m_TargetPosition = NullTargetPosition;
		m_IsSelected = false;
		m_CanBeSelected = true;
		m_InputMonitor = nullptr;

		GetRigidbody().SetStatic(false);
		GetRigidbody().SetGravityEnabled(false);
	}

	BActor::~BActor()
	{
		if (m_InputMonitor)
		{
			delete m_InputMonitor;
			m_InputMonitor = nullptr;
		}
	}

	const glm::vec3& BActor::GetTargetPosition() const
	{
		return m_TargetPosition;
	}

	void BActor::SetTargetPosition(const glm::vec3& position)
	{
		m_TargetPosition = position;
	}

	void BActor::SetCanBeSelected(const bool state)
	{
		m_CanBeSelected = state;
	}

	const bool BActor::CanBeSelected() const
	{
		return m_CanBeSelected;
	}

	void BActor::SetIsSelected(const bool state)
	{
		m_IsSelected = state;
	}

	const bool BActor::IsSelected() const
	{
		return m_IsSelected;
	}

	SphereCollider& BActor::GetSelectionCollider()
	{
		return m_SelectionCollider;
	}

	void BActor::Update(const float& deltaTime)
	{
		BProp::Update(deltaTime);

		m_SelectionCollider.Update(deltaTime);

		float movespeed = 10.0f * deltaTime;

		constexpr float maxSpeed = 2100.0f;
		constexpr glm::vec3 maxForce = glm::vec3(100.0f);
		
		if (m_TargetPosition != NullTargetPosition)
		{
			glm::vec3 newPosition = Helper::MoveTowards(GetTransform().GetPosition(), m_TargetPosition, movespeed);
			GetTransform().SetPosition(newPosition);
		
			glm::vec3 dir = glm::normalize(glm::vec3(m_TargetPosition - GetTransform().GetPosition()));
		
			if (Helper::Distance(GetTransform().GetPosition(), m_TargetPosition) < 0.025f)
			{
				m_TargetPosition = NullTargetPosition;
			}
		}
	}

	void BActor::Render(const Renderer& renderer)
	{
		BProp::Render(renderer);
		m_SelectionCollider.Render(renderer);
	}
}