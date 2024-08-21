#include <BennettPCH.h>
#include <Physics/Collision/Collider/Collider.h>
#include <System/Assets/AssetManager.h>

namespace Bennett
{
	Collider::Collider(const ColliderType& type, const Transform& transform, const glm::vec3& offset)
		: m_Transform(transform)
	{
		m_Type = type;
		m_Offset = offset;
		m_IsEnabled = true;
		SetModel();
	}

	const Transform& Collider::GetTransform() const
	{
		return m_Transform;
	}

	Model* Collider::GetModel() const
	{
		return m_Model;
	}

	Collider::~Collider()
	{
		m_Model = nullptr;
	}

	void Collider::SetEnabled(const bool& state)
	{
		m_IsEnabled = state;
	}

	const bool& Collider::IsEnabled() const
	{
		return m_IsEnabled;
	}

	void Collider::SetModel()
	{
		switch (m_Type)
		{
		default:
		case Bennett::ColliderType::Unknown:
			Log(LOG_MINIMAL, "Collider trying to set a model of an unknown type.\n");
			break;
		case Bennett::ColliderType::Sphere:
			m_Model = AssetManager::GetModel("1x1_Sphere");
			break;
		case Bennett::ColliderType::AABB:
		case Bennett::ColliderType::OBB:
			m_Model = AssetManager::GetModel("1x1_Cube");
			break;
		}
	}

	void Collider::Update(const float& deltaTime)
	{

	}
}