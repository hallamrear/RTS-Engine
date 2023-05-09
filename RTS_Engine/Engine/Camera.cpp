#include "BennettPCH.h"
#include "Camera.h"

Camera::Camera()
{
	NearPlaneDistance = 0.1f;
	FarPlaneDistance = 100.0f;
	AspectRatio = (1280.0f / 720.0f);
	FOVAngle = 45.0f;
	Position = glm::vec3(0.0f, 5.0f, -10.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::lookAt(Position, Position + glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	glm::mat4 projection = glm::perspective(FOVAngle, AspectRatio, NearPlaneDistance, FarPlaneDistance);
	//vulkan uses weird coordinates so multiply by -1.
	projection[1][1] *= -1;
	return projection;
}

void Camera::Translate(const glm::vec3 offset)
{
	Position += offset;
}

void Camera::Rotate(const glm::vec3 offset)
{
	Rotation += offset;
}
