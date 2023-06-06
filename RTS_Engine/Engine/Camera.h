#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>

namespace Bennett
{
	class CameraController;
	class InputMonitor;

	class Camera
	{
	protected:
		const CameraController& m_Controller;
		InputMonitor* m_InputMonitor;
		std::vector<int> m_CameraControlKeys =
		{
			GLFW_KEY_W,			 //Translate Forward
			GLFW_KEY_S,			 //Translate Backward
			GLFW_KEY_A,			 //Translate Left
			GLFW_KEY_D,			 //Translate Right
			GLFW_KEY_SPACE,		 //Translate Up
			GLFW_KEY_LEFT_SHIFT, //Translate Down
			GLFW_KEY_R,			 //Rotate Up
			GLFW_KEY_F,			 //Rotate Down
			GLFW_KEY_Q,			 //Rotate Left
			GLFW_KEY_E,			 //Rotate Right
		};

		Camera();

	public:
		float NearPlaneDistance;
		float FarPlaneDistance;
		float AspectRatio;
		float FOVAngle;
		glm::vec3 Position;
		glm::quat Rotation;

		float MovementSpeed = 20.0f;
		float RotationSpeed = 80.0f;

		virtual ~Camera() = 0;

		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetProjectionMatrix() = 0;

		virtual void ProcessInput(const float& deltaTime) = 0;
		void Translate(const glm::vec3& offset);
		void Rotate(const glm::vec3& offset);
	};
}