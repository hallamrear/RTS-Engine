#include "BennettPCH.h"
#include "Entity.h"
#include "Model.h"

namespace Bennett
{
	Entity::Entity(const std::string& name)
	{
		m_Name = name;
		Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Rotation = glm::quat();
		m_Speed = 1.0f;
	}

	Entity::Entity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation)
	{
		m_Name = name;
		Scale = scale;
		Position = position;
		Rotation = glm::quat(rotation);
	}

	Entity::~Entity()
	{

	}

	void Entity::Update(const float& DeltaTime)
	{
		Rotation = glm::rotate(Rotation, glm::vec3(0.0f, m_Speed * DeltaTime, 0.0f));
	}

	void Entity::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, Scale);
		glm::mat4 translate = glm::translate(matrix, Position);
		glm::mat4 rotate =glm::toMat4(Rotation);
		matrix = scale * translate * rotate;

		renderer.PushModelMatrix(matrix);

		_Model->Render(renderer);
	}

	const std::string& Entity::GetName() const
	{
		return m_Name;
	}
}