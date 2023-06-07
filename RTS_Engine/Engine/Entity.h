#pragma once

namespace Bennett
{
	class Model;
	class Renderer;

	class Entity
	{
	private:
		float m_Speed;
		std::string m_Name;

	protected:

	public:
		glm::vec3 Scale;
		glm::vec3 Position;
		glm::quat Rotation;

		Model* _Model;

		Entity(const std::string& name = "");
		Entity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation);
		~Entity();

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer);

		const std::string& GetName() const;
	};
}