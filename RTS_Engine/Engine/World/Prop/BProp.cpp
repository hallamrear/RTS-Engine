#include "BennettPCH.h"
#include "BProp.h"
#include <Defines/BennettInputDefines.h>
#include <Physics/Collision/Collider/AABBCollider.h>
#include <Physics/Collision/Collider/Collider.h>
#include <Physics/Collision/Collider/OBBCollider.h>
#include <Physics/Collision/Collider/SphereCollider.h>
#include <Rendering/Model.h>
#include <Rendering/Renderer.h>
#include <System/Assets/AssetManager.h>
#include <System/InputMonitor.h>
#include <System/Logger.h>

namespace Bennett
{
	BProp::BProp(const std::string& name, const Transform& transform) : BEntity(name, transform)
	{
		m_Collider = nullptr;
		m_Model = nullptr;
		m_Texture = nullptr;
	}

	BProp::~BProp()
	{
		DestroyCollider();

		//Model and Texture destruction is done by the AssetManager
		m_Model = nullptr;
		m_Texture = nullptr;
	}

	Model* BProp::GetModel()
	{
		return m_Model;
	}

	void BProp::SetModel(Model* model)
	{
		assert(model != nullptr);
		m_Model = model;
	}

	bool BProp::HasModel() const
	{
		return m_Model != nullptr;
	}

	const Texture& BProp::GetTexture() const
	{ 
		return *m_Texture;
	}

	void BProp::SetTexture(Texture* texture)
	{
		assert(texture != nullptr);
		m_Texture = texture;
	}

	bool BProp::HasTexture() const
	{
		return m_Texture != nullptr;
	}

	Collider* BProp::GetCollider() const
	{
		if (m_Collider)
			return m_Collider;
		
		return nullptr;
	}

	bool BProp::HasCollider() const
	{
		return m_Collider != nullptr;
	}

	void BProp::DestroyCollider()
	{
		if (m_Collider)
		{
			delete m_Collider;
			m_Collider = nullptr;
		}
	}
	void BProp::GenerateColliderFromModel(const ColliderType& type)
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
		AddCollider(type, extent, offset);
	}

	void BProp::AddCollider(const ColliderType& type, const glm::vec3& size, const glm::vec3& offset)
	{
		DestroyCollider();

		switch (type)
		{
		case Bennett::ColliderType::Sphere:
		{
			float r = std::fmaxf(size.x, size.y);
			r = std::fmaxf(r, size.z);
			m_Collider = new SphereCollider(GetTransform(), r, offset);
		}
		break;

		case Bennett::ColliderType::OBB:
		{
			m_Collider = new OBBCollider(GetTransform(), size, offset);
		}
		break;

		case Bennett::ColliderType::AABB:
		{
			m_Collider = new AABBCollider(GetTransform(), size, offset);
		}
		break;

		case Bennett::ColliderType::Unknown:
		default:
			Log(LOG_SERIOUS, "Attempted to create an unsupported collider type.\n");
			break;
		}
	}

	void BProp::Update(const float& deltaTime)
	{
		BEntity::Update(deltaTime);

		if (m_Collider)
		{
			m_Collider->Update(deltaTime);
		}
	}

	void BProp::Render(const Renderer& renderer)
	{
		BEntity::Render(renderer);

		if (HasTexture())
		{
			renderer.PushDescriptorSet(&GetTexture());
		}
		else
		{
			renderer.PushDescriptorSet(&renderer.GetDebugTexture());
		}

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
}