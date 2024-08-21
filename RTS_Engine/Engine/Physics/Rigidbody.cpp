#include "BennettPCH.h"
#include "Rigidbody.h"
#include <System/Transform.h>

namespace Bennett
{
	Rigidbody::Rigidbody(Transform& transform) : m_Transform(transform)
	{
		m_CentreOfMass = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		m_ForceSum = glm::vec3(0.0f, 0.0f, 0.0f);
		m_ImpulseSum = glm::vec3(0.0f, 0.0f, 0.0f);
		SetMass(RIGIDBODY_DEFAULT_MASS);

		m_Torque = glm::vec3(0.0f, 0.0f, 0.0f);
		m_AngularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		m_AngularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

		//This can remain as a default but should be alterable.
		float w = 10.0f, h = 10.0f, d = 10.0f;
		m_MomentOfInertia.x = (1.0f / 12.0f) * m_Mass * ((w * w) + (d * d));
		m_MomentOfInertia.y = (1.0f / 12.0f) * m_Mass * ((d * d) + (h * h));
		m_MomentOfInertia.z = (1.0f / 12.0f) * m_Mass * ((w * w) + (h * h));

		m_IsGravityEnabled = true;
		m_IsStatic = false;
	}

	Rigidbody::~Rigidbody()
	{

	}

	const bool& Rigidbody::IsGravityEnabled() const
	{
		return m_IsGravityEnabled;
	}

	void Rigidbody::SetGravityEnabled(const bool& gravityEnabled)
	{
		m_IsGravityEnabled = gravityEnabled;
	}

	const bool& Rigidbody::IsStatic() const
	{
		return m_IsStatic;
	}

	void Rigidbody::SetStatic(const bool& isStatic)
	{
		m_IsStatic = isStatic;
	}

	const glm::vec3& Rigidbody::GetVelocity() const
	{
		return m_Velocity;
	}

	const float& Rigidbody::GetMass() const
	{
		return m_Mass;
	}

	void Rigidbody::SetMass(const float& mass)
	{
		m_Mass = mass;
		m_InverseMass = 1.0f / mass;
	}

	const glm::vec3& Rigidbody::GetMomentOfInertia() const
	{
		return m_MomentOfInertia;
	}

	void Rigidbody::SetMomentOfInertia(const glm::vec3& moi)
	{
		m_MomentOfInertia = moi;
	}

	void Rigidbody::AddImpulseForce(const glm::vec3& force)
	{
		m_ImpulseSum += force;
	}

	Transform& Rigidbody::GetTransform()
	{
		return m_Transform;
	}

	void Rigidbody::Update(const float& deltaTime)
	{
		//Compute forces and apply forces.
		glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);
		if (m_IsGravityEnabled)
		{
			gravity = PHYSICS_GRAVITY_FORCE * m_Mass;
		}
		glm::vec3 friction = glm::vec3(0.0f);
		glm::vec3 drag = glm::vec3(0.0f);

		m_ForceSum = m_ImpulseSum + gravity + friction + drag;
		m_Torque = glm::cross(m_CentreOfMass, m_ForceSum);

		//Update Position and velocity.
		m_Acceleration = m_ForceSum / m_Mass;
		m_Velocity += m_Acceleration * deltaTime;
		m_Transform.Translate(m_Velocity * deltaTime);
		m_AngularAcceleration = m_Torque / m_MomentOfInertia;
		m_AngularVelocity += m_AngularAcceleration * deltaTime;
		m_Transform.Rotate(m_AngularVelocity);

		if (ENABLE_LOG_RIGIDBODY_DATA_PRINT)
		{
			Log(LOG_SAFE, "Force Sum: {%f, %f, %f}\n", m_ForceSum.x, m_ForceSum.y, m_ForceSum.z);
			Log(LOG_SAFE, "Acceleration: {%f, %f, %f}\n", m_Acceleration.x, m_Acceleration.y, m_Acceleration.z);
			Log(LOG_SAFE, "Velocity: {%f, %f, %f}\n", m_Velocity.x, m_Velocity.y, m_Velocity.z);
			Log(LOG_SAFE, "Angular Acceleration: {%f, %f, %f}\n", m_AngularAcceleration.x, m_AngularAcceleration.y, m_AngularAcceleration.z);
			Log(LOG_SAFE, "Angular Velocity: {%f, %f, %f}\n\n", m_AngularVelocity.x, m_AngularVelocity.y, m_AngularVelocity.z);
		}

		m_ImpulseSum = glm::vec3(0.0f, 0.0f, 0.0f); 
		m_ForceSum = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void Rigidbody::Render(const Renderer& renderer)
	{

	}
}