#include "BennettPCH.h"
#include "Entity.h"
#include "Model.h"

namespace Bennett
{

	Entity::Entity()
	{
		m_Speed = (rand() % 500 + 100);
	}

	Entity::~Entity()
	{

	}

	void Entity::Update(const float& deltaTime)
	{
		Rotation += m_Speed * deltaTime;

		if (Rotation > 360.0f) Rotation -= 360.0f;
	}

	void Entity::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(matrix, Position);
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = translate * rotate;

		renderer.PushModelMatrix(matrix);

		_Model->Render(renderer);
	}

}