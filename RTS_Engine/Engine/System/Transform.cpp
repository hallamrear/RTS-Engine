#include "BennettPCH.h"
#include "Transform.h"

namespace Bennett
{
	Transform::Transform()
	{
		m_Scale = glm::vec3(1.0f);
		m_Position = glm::vec3(0.0f);
		m_Rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
		UpdateBasisVectors();
	}

	Transform::Transform(const glm::vec3& scale, const glm::vec3& position, const glm::vec3& rotation)
	{
		m_Scale = scale;
		m_Position = position;
		m_Rotation = glm::quat(rotation);
		UpdateBasisVectors();
	}

	Transform::~Transform()
	{
		m_Scale = glm::vec3(0.0f);
		m_Position = glm::vec3(0.0f);
		m_Rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
	}

	void Transform::UpdateBasisVectors()
	{
		glm::vec3 euler = glm::eulerAngles(m_Rotation);
		m_ForwardVector.x = cos(glm::radians(euler.y)) * cos(glm::radians(euler.x));
		m_ForwardVector.y = sin(glm::radians(euler.x));
		m_ForwardVector.z = sin(glm::radians(euler.y)) * cos(glm::radians(euler.x));
		m_ForwardVector = glm::normalize(m_ForwardVector);
		m_RightVector = glm::normalize(glm::cross(m_ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_UpVector = glm::normalize(glm::cross(m_RightVector, m_ForwardVector));
	}

	glm::mat4x4 Transform::GetModelMatrix() const
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(matrix, m_Scale);
		glm::mat4 rotate = glm::toMat4(m_Rotation);
		glm::mat4 translate = glm::translate(matrix, m_Position);
		matrix = translate * rotate * scale;
		return matrix;
	}

	void Transform::SetPosition(const glm::vec3& position) 
	{ 
		m_Position = position; 
		UpdateBasisVectors();
	};

	void Transform::SetRotation(const glm::quat& quaternion)
	{ 
		m_Rotation = quaternion; 
		UpdateBasisVectors();
	}

	void Transform::SetRotation(const glm::vec3& eulerAngles)
	{ 
		m_Rotation = glm::quat(eulerAngles); 
		UpdateBasisVectors(); 
	};

	void Transform::SetScale(const glm::vec3& scale)
	{ 
		m_Scale = scale;
		UpdateBasisVectors();
	};

	void Transform::Translate(const glm::vec3& positionAdjustment)
	{
		m_Position += positionAdjustment;
		UpdateBasisVectors();
	};

	void Transform::Rotate(const glm::vec3& rotationAdjustmentEuler)
	{
		Rotate(glm::quat(glm::radians(rotationAdjustmentEuler)));
		UpdateBasisVectors();
	};

	void Transform::Rotate(const glm::quat& rotationAdjustmentQuat)
	{
		m_Rotation *= rotationAdjustmentQuat;
		UpdateBasisVectors();
	};

	void Transform::Scale(const glm::vec3& scaleAdjustment)
	{
		m_Scale += scaleAdjustment;
		UpdateBasisVectors();
	};

	const glm::vec3& Transform::GetForwardVector() const { return m_ForwardVector; };
	const glm::vec3& Transform::GetRightVector() const { return m_RightVector; };
	const glm::vec3& Transform::GetUpVector() const { return m_UpVector; };
	const glm::vec3& Transform::GetPosition()      const { return m_Position; };
	const glm::quat& Transform::GetRotation()      const { return m_Rotation; };
	const glm::vec3  Transform::GetRotationEulerRadians() const { return glm::eulerAngles(m_Rotation); };
	const glm::vec3  Transform::GetRotationEulerDegrees() const { return glm::degrees(glm::eulerAngles(m_Rotation)); };
	const glm::vec3& Transform::GetScale() const { return m_Scale; };
}