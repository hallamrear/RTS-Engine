#include <BennettPCH.h>
#include <World/Entity.h>
#include <Collision/Collider/Collider.h>
#include <Collision/Collider/SphereCollider.h>
#include <Collision/Collider/AABBCollider.h>
#include <System/InputMonitor.h>
#include <System/Manager/AssetManager.h>
#include <Defines/BennettInputDefines.h>

namespace Bennett
{
	Entity::Entity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation) 
	{
		//Setting data to stuff passed in.
		m_Name = name;
		m_Scale = scale;
		m_Position = position;
		m_Rotation = glm::quat(rotation);
	}

	Entity::~Entity()
	{
		if (inputMonitor)
		{
			delete inputMonitor;
			inputMonitor = nullptr;
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
		if (m_Collider)
		{
			m_Collider->Update(deltaTime);
		}
	}

	void Entity::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, m_Scale);
		glm::mat4 rotate = glm::toMat4(m_Rotation);
		glm::mat4 translate = glm::translate(matrix, m_Position);
		matrix = translate * rotate * scale;
		renderer.PushModelMatrix(matrix);

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

	void Entity::AddBroadPhaseCollider(ColliderType type, glm::vec3 size)
	{
		switch (type)
		{
		case Bennett::ColliderType::Unknown:
			break;
		case Bennett::ColliderType::Sphere:
			DestroyCollider();
			m_Collider = new SphereCollider(m_Position, size.x);
			break;
		case Bennett::ColliderType::OBB:
			DestroyCollider();
			break;
		case Bennett::ColliderType::AABB:
			DestroyCollider();
			m_Collider = new AABBCollider(m_Position, size);
			break;
		default:
			break;
		}
	}

	void Entity::AddNarrowPhaseCollider(ColliderType type, glm::vec3 size)
	{

	}

	void Entity::UpdateBasisVectors()
	{
		glm::vec3 euler = glm::eulerAngles(m_Rotation);
		m_ForwardVector.x = cos(glm::radians(euler.y)) * cos(glm::radians(euler.x));
		m_ForwardVector.y = sin(glm::radians(euler.x));
		m_ForwardVector.z = sin(glm::radians(euler.y)) * cos(glm::radians(euler.x));
		m_ForwardVector = glm::normalize(m_ForwardVector);
		m_RightVector = glm::normalize(glm::cross(m_ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_UpVector = glm::normalize(glm::cross(m_RightVector, m_ForwardVector));
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
