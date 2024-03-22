#include "BennettPCH.h"
#include "RigidbodyPhysics.h"
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

		//todo: Moment of inertia initialisation.
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

	const float& Rigidbody::GetMass() const
	{
		return m_Mass;
	}

	void Rigidbody::SetMass(const float& mass)
	{
		m_Mass = mass;
		m_InverseMass = 1.0f / mass;
	}

	void Rigidbody::AddImpulseForce(const glm::vec3& force)
	{
		m_ImpulseSum += force;
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

		printf("Force Sum: {%f, %f, %f}\n", m_ForceSum.x, m_ForceSum.y, m_ForceSum.z);
		printf("Acceleration: {%f, %f, %f}\n", m_Acceleration.x, m_Acceleration.y, m_Acceleration.z);
		printf("Velocity: {%f, %f, %f}\n", m_Velocity.x, m_Velocity.y, m_Velocity.z);
		printf("Angular Acceleration: {%f, %f, %f}\n", m_AngularAcceleration.x, m_AngularAcceleration.y, m_AngularAcceleration.z);
		printf("Angular Velocity: {%f, %f, %f}\n\n", m_AngularVelocity.x, m_AngularVelocity.y, m_AngularVelocity.z);

		m_ImpulseSum = glm::vec3(0.0f, 0.0f, 0.0f); 
		m_ForceSum = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void Rigidbody::Render(const Renderer& renderer)
	{

	}
}