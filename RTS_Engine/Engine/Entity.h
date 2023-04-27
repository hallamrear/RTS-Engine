#pragma once

namespace Bennett
{
	class Model;
	class Renderer;

	class Entity
	{
	private:
		float m_Speed;
	public:
		glm::vec3 Position;
		float Rotation;

		Model* _Model;

		Entity();
		~Entity();

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer);
	};
}