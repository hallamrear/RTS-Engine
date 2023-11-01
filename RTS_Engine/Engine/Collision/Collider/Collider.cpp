#include <BennettPCH.h>
#include <Collision/Collider/Collider.h>
#include <System/Manager/AssetManager.h>

namespace Bennett
{
	Collider::Collider(const ColliderType& type)
	{
		m_Type = type;
		SetModel();
	}

	Model* Collider::GetModel() const
	{
		return m_Model;
	}

	Collider::~Collider()
	{
		m_Model = nullptr;
	}

	void Collider::SetModel()
	{
		switch (m_Type)
		{
		default:
		case Bennett::ColliderType::Unknown:
			Log("Collider trying to set a model of an unknown type.", LOG_MINIMAL);
			break;
		case Bennett::ColliderType::Sphere:
			m_Model = AssetManager::GetModel("1x1_Sphere");
			break;
		case Bennett::ColliderType::OBB:
			m_Model = AssetManager::GetModel("1x1_Cube");
			break;
		case Bennett::ColliderType::AABB:
			m_Model = AssetManager::GetModel("1x1_Cube");
			break;
		}

	}
}