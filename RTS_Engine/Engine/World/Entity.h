#pragma once
#include <Defines/BennettDefines.h>
#include <glm/gtx/quaternion.hpp>
#include <Rendering/Model.h>
#include <string>

namespace Bennett
{
	enum class ColliderType;
	class Model;
	class Renderer;
	class Collider;

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
		Model* m_Model;
		Collider* m_Collider;

	protected:
		class InputMonitor* inputMonitor;
		virtual void UpdateBasisVectors();

		void DestroyCollider();

	public:
		Entity(const std::string& name,
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));

		virtual ~Entity();

		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer);

		inline const glm::vec3& GetForwardVector() const { return m_ForwardVector; };
		inline const glm::vec3& GetRightVector() const { return m_RightVector; };
		inline const glm::vec3& GetUpVector() const { return m_UpVector; };
		inline const glm::vec3& GetPosition()      const { return m_Position; };
		inline const glm::quat& GetRotation()      const { return m_Rotation; };
		inline const glm::vec3 GetRotationEulerRadians() const { return glm::eulerAngles(m_Rotation); };
		inline const glm::vec3 GetRotationEulerDegrees() const { return glm::degrees(glm::eulerAngles(m_Rotation)); };
		inline const glm::vec3& GetScale() const { return m_Scale; };
		inline Model* GetModel() { return m_Model; };

		inline void SetPosition(const glm::vec3& position) { m_Position = position; };
		inline void SetRotation(const glm::quat& quaternion) { m_Rotation = quaternion; };
		inline void SetRotationEuler(const glm::vec3& eulerAngles) { m_Rotation = glm::quat(eulerAngles); };
		inline void SetScale(const glm::vec3& scale) { m_Scale = scale; };
		void SetModel(Model* model);
		bool HasModel()   const { return m_Model != nullptr; };

		Collider* GetCollider() const { if (m_Collider) return m_Collider; else return nullptr; };
		bool HasCollider()  const { return m_Model != nullptr; };

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

		void AddBroadPhaseCollider(ColliderType type, glm::vec3 size);
		void AddNarrowPhaseCollider(ColliderType type, glm::vec3 size);
	};
}