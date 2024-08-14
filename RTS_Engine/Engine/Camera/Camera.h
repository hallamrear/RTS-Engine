#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Defines/BennettDefines.h"

namespace Bennett
{
	class CameraController;
	class InputMonitor;
	class Ray;

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
		bool m_MouseLookEnabled;

	public:
		virtual ~Camera() = 0;

		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetProjectionMatrix();

		float GetNearPlaneDistance() const;
		const bool& IsMouseLookEnabled() const;
		void SetMouseLookEnabled(const bool& state);
		float GetFarPlaneDistance() const;
		float GetAspectRatio() const;
		float GetFOVAngleRadians() const;
		float GetFOVAngleDegrees() const;
		float GetMovementSpeed() const;
		float GetRotationSpeed() const;
		const glm::vec3& GetPosition() const;
		const glm::vec3& GetRotation() const;
		inline const glm::vec3& GetForwardVector() const { return m_ForwardVector; };
		inline const glm::vec3& GetRightVector() const { return m_RightVector; };
		inline const glm::vec3& GetUpVector() const { return m_UpVector; };

		void SetNearPlaneDistance(const float& nearPlane);
		void SetFarPlaneDistance(const float& farPlane);
		void SetAspectRatio(const float& aspectRatio);
		void SetFOVAngle(const float& fovAngleDegrees);
		void SetMovementSpeed(const float& movementSpeed);
		void SetRotationSpeed(const float& rotationSpeed);
		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);

		virtual void Update(const float& deltaTime) = 0;
		void Translate(const glm::vec3& offset);
		void Rotate(const glm::vec3& offset);

		/// <summary>
		/// Raycasts outward from the camera returning a ray in worldspace.
		/// </summary>
		/// <param name="ndcClickCoords">The position of the raycast in Normalised Device Coordinates.</param>
		/// <returns>Worldspace Ray from the camera's position point.</returns>
		Ray Raycast(const glm::vec2& ndcClickCoords);
		
		/// <summary>
		/// Raycasts outward from the camera returning a ray in worldspace. This is the same as Raycast(NDC) when clicking the centre of the viewport.
		/// </summary>
		/// <returns>Worldspace Ray from the camera's position point in the camera's forward direction.</returns>
		Ray Raycast();
	};
}