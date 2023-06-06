#include "BennettPCH.h"
#include "Camera.h"
#include "CameraController.h"
#include "InputMonitor.h"

namespace Bennett
{
	Camera::Camera() : m_Controller(CameraController::Get())
	{
		NearPlaneDistance = 0.1f;
		FarPlaneDistance = 1000.0f;
		AspectRatio = (1280.0f / 720.0f);
		FOVAngle = 45.0f;
		Position = glm::vec3(0.0f, 5.0f, -10.0f);
		Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		m_InputMonitor = new InputMonitor(m_CameraControlKeys);
	}

	Camera::~Camera()
	{

	}

	//glm::mat4 Camera::GetViewMatrix()
	//{
	//	glm::mat4 view = glm::quatLookAt(Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	//	return view;
	//}

	//glm::mat4 Camera::GetProjectionMatrix()
	//{
	//	glm::mat4 projection = glm::perspective(FOVAngle, AspectRatio, NearPlaneDistance, FarPlaneDistance);
	//	//vulkan uses weird coordinates so multiply by -1.
	//	projection[1][1] *= -1;
	//	return projection;
	//}

	void Camera::ProcessInput(const float& deltaTime)
	{

	}

	void Camera::Translate(const glm::vec3& offset)
	{
		Position += offset;
	}

	void Camera::Rotate(const glm::vec3& offset)
	{
		glm::vec3 rotationRadians = glm::radians(offset);
		glm::quat offsetQuat = glm::quat(rotationRadians);
		Rotation = offsetQuat * Rotation;
		printf("%f %f %f %f\n", Rotation.x, Rotation.y, Rotation.z, Rotation.w);
	}
}