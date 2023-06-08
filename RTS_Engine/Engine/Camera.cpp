#include "BennettPCH.h"
#include "Camera.h"
#include "CameraController.h"
#include "InputMonitor.h"

namespace Bennett
{
	Camera::Camera() : m_Controller(CameraController::Get())
	{
		m_ForwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
		m_RightVector   = glm::vec3(1.0f, 0.0f, 0.0f);
		m_UpVector      = glm::vec3(0.0f, 1.0f, 0.0f);
		m_NearPlaneDistance = 0.1f;
		m_FarPlaneDistance = 1000.0f;
		m_AspectRatio = (1280.0f / 720.0f);
		m_FOVAngle = glm::radians(60.0f);
		m_Position = glm::vec3(0.0f, 5.0f, -10.0f);
		m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_MovementSpeed = 20.0f;
		m_RotationSpeed = 80.0f;

		m_InputMonitor = new InputMonitor(m_CameraControlKeys);
	}

	void Camera::UpdateBasisVectors()
	{
		m_ForwardVector.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
		m_ForwardVector.y = sin(glm::radians(m_Rotation.x));
		m_ForwardVector.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
		m_ForwardVector = glm::normalize(m_ForwardVector);
		m_RightVector = glm::normalize(glm::cross(m_ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_UpVector = glm::normalize(glm::cross(m_RightVector, m_ForwardVector));
	}

	Camera::~Camera()
	{

	}

	void Camera::Translate(const glm::vec3& offset)
	{
		m_Position += offset;
	}

	void Camera::Rotate(const glm::vec3& offset)
	{
		m_Rotation += offset;
		ClampRotations();
		UpdateBasisVectors();
	}

	const glm::vec3& Camera::GetRotation() const
	{
		return m_Rotation;
	}

	const glm::vec3& Camera::GetPosition() const
	{
		return m_Position;
	}

	void Camera::ProcessInput(const float& deltaTime)
	{

	}

	float Camera::GetAspectRatio() const
	{
		return m_AspectRatio;
	}

	float Camera::GetFOVAngleRadians() const
	{
		return m_FOVAngle;
	}

	float Camera::GetFOVAngleDegrees() const
	{
		return glm::degrees(m_FOVAngle);
	}

	float Camera::GetMovementSpeed() const
	{
		return m_MovementSpeed;
	}

	float Camera::GetRotationSpeed() const
	{
		return m_RotationSpeed;
	}

	void Camera::SetNearPlaneDistance(const float& nearPlane) 
	{
		m_NearPlaneDistance = nearPlane;
	}

	void Camera::SetFarPlaneDistance(const float& farPlane) 
	{
		m_FarPlaneDistance = farPlane;
	}

	void Camera::SetAspectRatio(const float& aspectRatio) 
	{
		m_AspectRatio = aspectRatio;
	}

	void Camera::SetFOVAngle(const float& fovAngleDegrees) 
	{
		m_FOVAngle = glm::radians(fovAngleDegrees);
	}

	void Camera::SetMovementSpeed(const float& movementSpeed) 
	{
		m_MovementSpeed = movementSpeed;
	}

	void Camera::SetRotationSpeed(const float& rotationSpeed)
	{
		m_RotationSpeed = rotationSpeed;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{ 
		m_Rotation = rotation;
	}

	float Camera::GetFarPlaneDistance() const
	{
		return m_FarPlaneDistance;
	}

	float Camera::GetNearPlaneDistance() const
	{
		return m_NearPlaneDistance;
	}

	void Camera::ClampRotations()
	{
		if (m_Rotation.y > 360.0f)
			m_Rotation.y -= 360.0f;
		if (m_Rotation.y < 0.0f)
			m_Rotation.y += 360.0f;

		if (m_Rotation.x >  90.0f)
			m_Rotation.x =  89.0f;
		if (m_Rotation.x < -90.0f)
			m_Rotation.x = -89.0f;
	}
}