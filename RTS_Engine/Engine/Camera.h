#pragma once
class Camera
{
private:
public:
	float NearPlaneDistance;
	float FarPlaneDistance;
	float AspectRatio;
	float FOVAngle;
	glm::vec3 Position;
	glm::vec3 Rotation;

	Camera();
	~Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void Translate(const glm::vec3 offset);
	void Rotate(const glm::vec3 offset);
};

