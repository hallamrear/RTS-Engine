#include "BennettPCH.h"
#include "Entity.h"
#include "Model.h"
#include "Texture.h"
#include "AssetManager.h"

namespace Bennett
{
	Entity::Entity(const std::string& name)
	{
		m_Name = name;
		m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Rotation = glm::identity<glm::quat>();
	}

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

	void Entity::Render(Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, m_Scale);
		glm::mat4 translate = glm::translate(matrix, m_Position);
		glm::mat4 rotate = glm::toMat4(m_Rotation);
		matrix = scale * translate * rotate;
		renderer.PushModelMatrix(matrix);
		
		if (m_Texture != nullptr)
			renderer.BindTexture(GetTexture());

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
}