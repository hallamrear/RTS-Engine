#include "BennettPCH.h"
#include "Entity.h"
#include "Model.h"
#include "Texture.h"
#include "AssetManager.h"

namespace Bennett
{
	Entity::Entity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation)
	{
		m_Name = name;
		m_Scale = scale;
		m_Position = position;
		m_Rotation = glm::quat(rotation);
	}

	Entity::~Entity()
	{
		m_Model = nullptr;
	}

	void Entity::Update(const float& DeltaTime)
	{

	}

	void Entity::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, m_Scale);
		glm::mat4 translate = glm::translate(matrix, m_Position);
		glm::mat4 rotate = glm::toMat4(m_Rotation);
		matrix = scale * translate * rotate;
		renderer.PushModelMatrix(matrix);
	
		if(m_Texture)
			renderer.PushTextureID(m_Texture->GetID());

		if(m_Model != nullptr)
			m_Model->Render(renderer);
	}

	void Entity::SetModel(const Model* model)
	{
		m_Model = model;
	}

	void Entity::SetTexture(const Texture* texture)
	{
		m_Texture = texture;
	}

	const std::string& Entity::GetName() const
	{
		return m_Name;
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
}

Bennett::Entity::Entity(const std::string& m_Name)
	: m_Name(m_Name)
{
}
