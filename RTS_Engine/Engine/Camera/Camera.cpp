#include <BennettPCH.h>
#include <Camera/Camera.h>
#include <Camera/CameraController.h>
#include <System/InputMonitor.h>
#include <System/ServiceLocator.h>
#include <Rendering/Window.h>
#include <Collision/Collider/Ray.h>

namespace Bennett
{
	Camera::Camera() : m_Controller(CameraController::Get())
	{
		m_MouseLookEnabled = true;
		m_ForwardVector = BENNETT_FORWARD_VECTOR;;
		m_RightVector   = BENNETT_RIGHT_VECTOR;
		m_UpVector      = BENNETT_UP_VECTOR;
		m_NearPlaneDistance = 0.1f;
		m_FarPlaneDistance = 10000.0f;
		m_AspectRatio = (1280.0f / 720.0f);
		m_FOVAngle = glm::radians(60.0f);
		m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_MovementSpeed = 20.0f;
		m_RotationSpeed = 80.0f;

		m_CameraControlKeys =
		{
			BENNETT_KEY_W,			 //Translate Forward
			BENNETT_KEY_S,			 //Translate Backward
			BENNETT_KEY_A,			 //Translate Left
			BENNETT_KEY_D,			 //Translate Right
			BENNETT_KEY_SPACE,		 //Translate Up
			BENNETT_KEY_LSHIFT,		 //Translate Down
			BENNETT_KEY_R,			 //Rotate Up
			BENNETT_KEY_F,			 //Rotate Down
			BENNETT_KEY_Q,			 //Rotate Left
			BENNETT_KEY_E,			 //Rotate Right
			BENNETT_KEY_TAB,		 //Toggle Mouse Lock
			BENNETT_KEY_HOME,		 //Movement speed up
			BENNETT_KEY_END,		 //Movement speed down
			BENNETT_KEY_PRIOR,		 //Rotation speed up
			BENNETT_KEY_NEXT,		 //Rotation speed down
		};

		m_InputMonitor = new InputMonitor(m_CameraControlKeys);
	}

	Camera::~Camera()
	{
		if (m_InputMonitor)
		{
			delete m_InputMonitor;
			m_InputMonitor = nullptr;
		}
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

	void Camera::Translate(const glm::vec3& offset)
	{
		m_Position += offset;

		if (ENABLE_LOG_CAMERA_LOCATION_PRINT)
		{
			Log(LOG_STATUS::LOG_SAFE, "Camera Position: %f, %f, %f\n", m_Position.x, m_Position.y, m_Position.z);
		}
	}

	void Camera::Rotate(const glm::vec3& offset)
	{
		m_Rotation += offset;
		ClampRotations();
		UpdateBasisVectors();

		if (ENABLE_LOG_CAMERA_LOCATION_PRINT)
		{
			Log(LOG_STATUS::LOG_SAFE, "Camera Rotation: %f, %f, %f\n", m_Rotation.x, m_Rotation.y, m_Rotation.z);
		}
	}

	Ray Camera::Raycast(const glm::vec2& ndcClickCoords)
	{
		//Using mouse position in NDC coordinates.
		//Want the ray to be pointing fowards (down the Z axis) and want to ensure its treated as a ray.
		glm::vec4 projectionSpaceRay = glm::vec4(ndcClickCoords.x, ndcClickCoords.y, -1.0f, 1.0f);

		//Reverse from projection space back into view space by using the inverse projection.
		//Only need to unproject the X and Y so we redefine the ray looking forwards and set W to 0.0 to treat as a point.
		glm::vec4 viewSpaceRay = glm::inverse(GetProjectionMatrix()) * projectionSpaceRay;
		viewSpaceRay.z = -1.0f;
		viewSpaceRay.w = 0.0f;

		glm::vec4 worldSpaceRay = glm::vec4((glm::inverse(GetViewMatrix()) * viewSpaceRay));
		glm::vec3 direction = glm::normalize(glm::vec3(worldSpaceRay));
	
		return Ray(GetPosition(), direction);
	}

	Ray Camera::Raycast()
	{
		return Raycast(glm::vec2(0.0));
	}

	const glm::vec3& Camera::GetRotation() const
	{
		return m_Rotation;
	}

	const glm::vec3& Camera::GetPosition() const
	{
		return m_Position;
	}

	void Camera::Update(const float& deltaTime)
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

		if (ENABLE_LOG_CAMERA_LOCATION_PRINT)
		{
			Log(LOG_STATUS::LOG_SAFE, "Camera Position: %f, %f, %f\n", m_Position.x, m_Position.y, m_Position.z);
		}
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{ 
		m_Rotation = rotation;

		if (ENABLE_LOG_CAMERA_LOCATION_PRINT)
		{
			Log(LOG_STATUS::LOG_SAFE, "Camera Rotation: %f, %f, %f\n", m_Rotation.x, m_Rotation.y, m_Rotation.z);
		}
	}

	float Camera::GetFarPlaneDistance() const
	{
		return m_FarPlaneDistance;
	}

	float Camera::GetNearPlaneDistance() const
	{
		return m_NearPlaneDistance;
	}


	const bool& Camera::IsMouseLookEnabled() const
	{
		return m_MouseLookEnabled;
	}

	void Camera::SetMouseLookEnabled(const bool& state)
	{
		m_MouseLookEnabled = state;
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

	glm::mat4 Camera::GetProjectionMatrix()
	{
		glm::vec2 size = ServiceLocator::GetWindow().GetSize();

		if (size.x == 0.0f && size.y == 0.0f)
		{
			size.x = 1.0f;
			size.y = 1.0f;
		}

		m_AspectRatio = size.x / size.y;

		/*
		Vulkan clip space has [1, -1] for the y axis where OpenGL has [-1, 1] 
		so just multiplying the perspective matrix [1][1] value by -1 will "correct"
		the OpenGL matrix to work with Vulkan.
		
		All it really does is flip the y axis. (or you can also use gl_Postion.y *= -1)
		*/
		glm::mat4 projection = glm::perspective(m_FOVAngle, m_AspectRatio, m_NearPlaneDistance, m_FarPlaneDistance);
		projection[1][1] *= -1;
		return projection;
	}
}