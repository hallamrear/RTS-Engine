#pragma once
#include <glm/gtx/quaternion.hpp>

namespace Bennett
{
	class BENNETT_ENGINE Transform
	{
	private:
		glm::vec3 m_Scale;
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_ForwardVector;
		glm::vec3 m_LeftVector;
		glm::vec3 m_UpVector;

	public:
		Transform();
		Transform(const glm::vec3& scale, const glm::vec3& position, const glm::vec3& rotation);
		~Transform();

		void UpdateBasisVectors();

		glm::mat4x4 GetModelMatrix() const;
		glm::mat4x4	GetRotationMatrix() const;

		const glm::vec3& GetForwardVector() const;
		const glm::vec3& GetLeftVector() const;
		const glm::vec3& GetUpVector() const;
		const glm::vec3& GetPosition() const;
		const glm::quat& GetRotation() const;
		const glm::vec3  GetRotationEulerRadians() const;
		const glm::vec3  GetRotationEulerDegrees() const;
		const glm::vec3& GetScale() const;

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::quat& quaternion);
		void SetRotation(const glm::vec3& eulerAngles);
		void SetScale(const glm::vec3& scale);
		void Translate(const glm::vec3& positionAdjustment);
		void Rotate(const glm::vec3& rotationAdjustmentEuler);
		void Rotate(const glm::quat& rotationAdjustmentQuat);
		void Scale(const glm::vec3& scaleAdjustment);
	};
}
