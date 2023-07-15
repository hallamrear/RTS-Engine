#pragma once
#define BENNETT_ENGINE __declspec(dllexport)
#include <string>
#include <glm/gtx/quaternion.hpp>

namespace Bennett
{
	class Model;
	class Texture;
	class Renderer;

	class BENNETT_ENGINE Entity
	{
	private:
		std::string m_Name;
		glm::vec3 m_Scale;
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_ForwardVector;
		glm::vec3 m_RightVector;
		glm::vec3 m_UpVector;
		const Model* m_Model;
		const Texture* m_Texture;

	protected:
		void UpdateBasisVectors();

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
		inline const Model& GetModel()			   const { return *m_Model; };
		inline const Texture& GetTexture()			   const { return *m_Texture; };

		inline void SetPosition(const glm::vec3& position) { m_Position = position; };
		inline void SetRotation(const glm::quat& quaternion) { m_Rotation = quaternion; };
		inline void SetRotationEuler(const glm::vec3& eulerAngles) { m_Rotation = glm::quat(eulerAngles); };
		inline void SetScale(const glm::vec3& scale) { m_Scale += scale; };
		void SetModel(const Model* modelName);
		void SetTexture(const Texture* texture);

		inline void Translate(const glm::vec3& positionAdjustment)
		{ 
			m_Position += positionAdjustment; 
			UpdateBasisVectors();
		};

		inline void Rotate(const glm::vec3& rotationAdjustmentEuler)
		{
			Rotate(glm::quat(glm::radians(rotationAdjustmentEuler)));
			UpdateBasisVectors();
		};

		inline void Rotate(const glm::quat& rotationAdjustmentQuat)
		{ 
			m_Rotation *= rotationAdjustmentQuat; 
			UpdateBasisVectors();
		};

		inline void Scale(const glm::vec3& scaleAdjustment) 
		{
			m_Scale += scaleAdjustment;
		};

		const std::string& GetName() const;
	};
}