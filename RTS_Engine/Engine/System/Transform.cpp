#include "BennettPCH.h"
#include "Transform.h"
#include <glm/gtc/quaternion.hpp>

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
		m_Rotation = glm::quat(glm::radians(rotation));
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
		glm::mat4 rotation = (GetRotationMatrix());
		glm::mat4 identity = glm::mat4(1.0f);
		m_ForwardVector = glm::normalize(rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		m_LeftVector = glm::normalize(rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		m_UpVector = glm::normalize(rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
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

	glm::mat4x4 Transform::GetRotationMatrix() const
	{
		return glm::toMat4(m_Rotation);
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
		m_Rotation = glm::quat(glm::radians(eulerAngles)); 
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

	/// <summary>
	/// Rotate the transform by the vec3 euler angles (in degrees).
	/// </summary>
	/// <param name="rotationAdjustmentEuler"></param>
	void Transform::Rotate(const glm::vec3& rotationAdjustmentEuler)
	{
		Rotate(glm::quat(glm::radians(rotationAdjustmentEuler)));
		UpdateBasisVectors();
	};

	void Transform::Rotate(const glm::quat& rotationAdjustmentQuat)
	{
		m_Rotation *= rotationAdjustmentQuat;
		m_Rotation = glm::normalize(m_Rotation);
		UpdateBasisVectors();
	};

	void Transform::Scale(const glm::vec3& scaleAdjustment)
	{
		m_Scale += scaleAdjustment;
		UpdateBasisVectors();
	};

	const glm::vec3& Transform::GetForwardVector() const { return m_ForwardVector; };
	const glm::vec3& Transform::GetLeftVector() const { return m_LeftVector; };
	const glm::vec3& Transform::GetUpVector() const { return m_UpVector; };
	const glm::vec3& Transform::GetPosition()      const { return m_Position; };
	const glm::quat& Transform::GetRotation()      const { return m_Rotation; };
	const glm::vec3  Transform::GetRotationEulerRadians() const { return glm::eulerAngles(m_Rotation); };
	const glm::vec3  Transform::GetRotationEulerDegrees() const { return glm::degrees(glm::eulerAngles(m_Rotation)); };
	const glm::vec3& Transform::GetScale() const { return m_Scale; };
}