#pragma once

namespace Bennett
{
	class Model;
	class Renderer;

	class Entity
	{
	private:
		std::string m_Name;

		glm::vec3 m_Scale;
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		const Model* m_Model;

	protected:

	public:
		Entity(const std::string& name = "");
		Entity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation);
		virtual ~Entity();

		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer);

		inline const glm::vec3& GetPosition()      const { return m_Position; };
		inline const glm::quat& GetRotation()      const { return m_Rotation; };
		inline const glm::vec3& GetRotationEulerRadians() const { return glm::eulerAngles(m_Rotation); };
		inline const glm::vec3& GetRotationEulerDegrees() const { return glm::degrees(glm::eulerAngles(m_Rotation)); };
		inline const glm::vec3& GetScale()		   const { return m_Scale; };
		inline const Model&     GetModel()		   const { return *m_Model; };

		inline void SetPosition(const glm::vec3& position) { m_Position = position; };
		inline void SetRotation(const glm::quat& quaternion) { m_Rotation = quaternion; };
		inline void SetRotationEuler(const glm::vec3& eulerAngles) { m_Rotation = glm::quat(eulerAngles); };
		inline void SetScale(const glm::vec3& scale) { m_Scale += scale; };
		void SetModel(const Model* modelName);

		inline void Translate(const glm::vec3& positionAdjustment) { m_Position += positionAdjustment; };

		inline void Rotate(const glm::vec3& rotationAdjustmentEuler)
		{
			m_Rotation = m_Rotation * glm::quat(glm::radians(rotationAdjustmentEuler));
		};

		inline void Rotate(const glm::quat& rotationAdjustmentQuat) { m_Rotation *= rotationAdjustmentQuat; };
		inline void Scale(const glm::vec3& scaleAdjustment) { m_Scale += scaleAdjustment; };

		const std::string& GetName() const;
	};
}