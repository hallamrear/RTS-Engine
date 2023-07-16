#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>

namespace Bennett
{
	class CameraController;
	class InputMonitor;

	class BENNETT_ENGINE Camera
	{
	protected:
		const CameraController& m_Controller;
		InputMonitor* m_InputMonitor;
		std::vector<int> m_CameraControlKeys;

		glm::vec3 m_ForwardVector;
		glm::vec3 m_RightVector;
		glm::vec3 m_UpVector;

		float m_NearPlaneDistance;
		float m_FarPlaneDistance;
		float m_AspectRatio;
		float m_FOVAngle;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		float m_MovementSpeed;
		float m_RotationSpeed;

		Camera();

		void UpdateBasisVectors();
		void ClampRotations();

	public:

		virtual ~Camera() = 0;

		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetProjectionMatrix() = 0;

		float GetNearPlaneDistance() const;
		float GetFarPlaneDistance() const;
		float GetAspectRatio() const;
		float GetFOVAngleRadians() const;
		float GetFOVAngleDegrees() const;
		float GetMovementSpeed() const;
		float GetRotationSpeed() const;
		const glm::vec3& GetPosition() const;
		const glm::vec3& GetRotation() const;

		void SetNearPlaneDistance(const float& nearPlane);
		void SetFarPlaneDistance(const float& farPlane);
		void SetAspectRatio(const float& aspectRatio);
		void SetFOVAngle(const float& fovAngleDegrees);
		void SetMovementSpeed(const float& movementSpeed);
		void SetRotationSpeed(const float& rotationSpeed);
		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);

		virtual void ProcessInput(const float& deltaTime) = 0;
		void Translate(const glm::vec3& offset);
		void Rotate(const glm::vec3& offset);
	};
}