#include <BennettPCH.h>
#include <World/Entity.h>
#include <Collision/Collider/Collider.h>
#include <Collision/Collider/SphereCollider.h>
#include <Collision/Collider/AABBCollider.h>
#include <Collision/Collider/OBBCollider.h>
#include <System/InputMonitor.h>
#include <System/Manager/AssetManager.h>
#include <Defines/BennettInputDefines.h>
#include <System/Logger.h>

namespace Bennett
{
	Entity::Entity(const std::string& name, const glm::vec3& scale, const glm::vec3& position, const glm::vec3& rotation)
	{
		//Setting data to stuff passed in.
		m_Name = name;
		m_Transform = Transform(scale, position, rotation);
	}

	Entity::Entity(const std::string& name, const Transform& transform)
	{
		m_Name = name;
		m_Transform = transform;
	}

	Entity::~Entity()
	{
		if (m_InputMonitor)
		{
			delete m_InputMonitor;
			m_InputMonitor = nullptr;
		}

		if (m_Collider)
		{
			delete m_Collider;
			m_Collider = nullptr;
		}

		//Model and Texture destruction is done by the AssetManager
		m_Model = nullptr;
	}

	void Entity::Update(const float& deltaTime)
	{
		m_Transform.UpdateBasisVectors();

		if (m_Collider)
		{
			m_Collider->Update(deltaTime);
		}
	}

	void Entity::Render(const Renderer& renderer)
	{
		glm::mat4 model = m_Transform.GetModelMatrix();
		renderer.PushConstants.ModelMatrix = model;
		renderer.UpdatePushConstants();

		if (m_Model != nullptr)
		{
			m_Model->Render(renderer);
		}

		if (m_Collider)
		{
			m_Collider->Render(renderer);
		}
	}

	void Entity::SetModel(Model* model)
	{
		m_Model = model;
	}

	const std::string& Entity::GetName() const
	{
		return m_Name;
	}

	void Entity::GenerateBroadPhaseColliderFromModel(const ColliderType& type)
	{
		if (m_Model == nullptr)
		{
			Log(LOG_SERIOUS, "Attempted to create a broad phase collider from model but no model is set.\n");
			return;
		}

		auto& meshes = m_Model->GetMeshes();
		glm::vec3 max = meshes[0]->GetMaxExtents();
		glm::vec3 min = meshes[0]->GetMinExtents();
		glm::vec3 extent = max - min;
		glm::vec3 offset = (max + min) / 2.0f;
		AddBroadPhaseCollider(type, extent, offset);
	}

	void Entity::AddBroadPhaseCollider(const ColliderType& type, const glm::vec3& size, const glm::vec3& offset)
	{
		DestroyCollider();

		switch (type)
		{
		case Bennett::ColliderType::Sphere:
		{
			float r = std::fmaxf(size.x, size.y);
			r = std::fmaxf(r, size.z);
			m_Collider = new SphereCollider(m_Transform, r, offset);
		}
			break;
		case Bennett::ColliderType::OBB:
		{
			m_Collider = new OBBCollider(m_Transform, size, offset);
		}
			break;
		case Bennett::ColliderType::AABB:
		{
			m_Collider = new AABBCollider(m_Transform, size, offset);
		}
			break;
		case Bennett::ColliderType::Unknown:
		default:
			Log(LOG_SERIOUS, "Attempted to create an unsupported collider type.\n");
			break;
		}
	}

	void Entity::AddNarrowPhaseCollider(const ColliderType& type, const glm::vec3& size, const glm::vec3& offset)
	{
		
	}

	void Entity::DestroyCollider()
	{
		if (m_Collider)
		{
			delete m_Collider;
			m_Collider = nullptr;
		}
	}
}
