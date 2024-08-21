#pragma once

namespace Bennett
{
	class Renderer;
	class Transform;

	class BENNETT_ENGINE Rigidbody
	{
	private:
		Transform& m_Transform;
		glm::vec3 m_CentreOfMass;
		glm::vec3 m_Velocity;
		glm::vec3 m_Acceleration;
		float m_Mass;
		float m_InverseMass;
		glm::vec3 m_ForceSum;
		glm::vec3 m_ImpulseSum;
		
		//Rotational Data
		glm::vec3 m_MomentOfInertia;
		glm::vec3 m_Torque;
		glm::vec3 m_AngularAcceleration;
		glm::vec3 m_AngularVelocity;
		bool m_IsGravityEnabled;
		bool m_IsStatic;

	public:
		Rigidbody(Transform& entity);
		~Rigidbody();

		const bool& IsGravityEnabled() const;
		void SetGravityEnabled(const bool& gravityEnabled);

		const bool& IsStatic() const;
		void SetStatic(const bool& isStatic);

		const glm::vec3& GetVelocity() const;
		const float& GetMass() const;
		void SetMass(const float& mass);

		const glm::vec3& GetMomentOfInertia() const;
		void SetMomentOfInertia(const glm::vec3& moi);

		void AddImpulseForce(const glm::vec3& force);

		Transform& GetTransform();

		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer);
	};
}

/*
VECTOR.
		express as x and y compoinents
		find mag using pythag
		find theta using arctan(y/x)

		find x comp from mag * cos theta
		find y comp from mag * sin theta

KINEMATICS
	movement wihtout forces

	requires 4 thiungs
	position m - where is located
	velocity m/s - how position changes over time
	acc - m/s^2 - how velocoty changes over time
	time - s - time elapsed since start

	Final pos = (1/2)(a)(dt)^2 + v(dt) + initial pos--
	0.5f * acc * time * time + velocity * time + position

	much more likely application of kinematics in game dev is in 2d
	
	

REGULAR FORCES
	mass = amount of matter (kg)
	1kg = 2.2lbs

	mass != weight
	weight is mass affected by gravity

	NEWTONS SECOND LAW
		F = ma
		N = kg * m/s

		unity.addforce should be called in fixed update.

		update is caleed a variable amount of time each seond
		Fixed updates are called at fixed intervals during each second.

		F = ma
		vector = scaler * vector

	NORMAL FORCE (gravity)

		if a box is on a ramp, where is the normal force
		it is acting directly down  onthe object.

		using trig you acn determine from the gravity downward force (Fg = mg)
	
		the two components to discover the x and y components
		can be determined as Fg1 (force acting downward from the object)
		and Fg2 (force acting in the direciton of the slop.

		theta is the angle of the slope.

		Fg1 = mg cos (theta)
		Fg2 = mg sin (theta)

		FORCES > KINEMATICS

	Friction & Air Resistance
		force that resists motion with forces in contact.

		F = mewN
		F = coeffectint of friction (0-1 usually) * Force Normal

		STATIC FRICTION IF AT REST,

		KINETIC FRICTION WHEN IN MOTION

		static is usually greater than kinetic friction.

		usually the coeff is produced from a sum of both coeffs/

		
		using friction equation we can use Fg1 and 2 as the force normal.

		Friction = mew * (mg * cos(theta))
				 = mew * Fg1

		Force SUm = Fg2 - FrictionForce.

		Coefficient of friction must be less than the tangent of the ground angle (theta)
		in order ot make the box move.
		Thsi works for any mass as mg's will cancel out.

	Drag
		Force that resists motion when object is in freefall (in air)

		Fd = 1/2(roe) * v^2 * Cd * A.

		Drag 1/2 * density * velocity^2 * drag coeff * area

		we can use somthing simpler instead.
		Fd = -bV

		BIG B = BIG DRAG

		force of frag is directly proportional to coeff

		tv = constant vel that a free falling object approaches as the resistance of the medium
		it is falling in approaches gravity.

IMPULSE FORCES

	MOMENTUM 

	mom = mass x vel
	p = mv

	units is kgm/s

	momentum is always conserved
	momentum before a collision is the same as the momentum after a collision

	m1v1 + m2v2 = m1pv1p + m2v2p

	elasticity of collision
		elastic collision - no energy is lost
		inelastic collision - some energy is lost
		perfectly inelastic - all energy is lost.

	bounciness is the 0-1 value that describes the elasticity of a collision on a physics material.

INSTANTANEOUS FORCE
	Impulse
		denoted with J 
		FdT = delta Momentum
		J = f(dt);

		to do realistically;
		1) figure out change in momentum (deltaP)
		2) come up with deltaT
		3) calculate force with J = F(dt)

		then setup timer that counts down from DT and 
		constantly adds the force to the object over interval

		BUT FOR GAMESSS ITS EASIER TO:
		Vf = (J/m) + Vi


ROTATIONAL MOTION

	ROTATIONAL KINEMATICS
		theta - just the angle, how rotated an angle is (deg or radians)
		omega - how the angle changes over time (deg or radians over time) (angular velocity)
		alpha - describes how omega changes over time (deg/radians per second^2) (angular acceleration)

		can replace variables in main kinematic equation to get an accurate assumption for rotational kinematics

		thetaF = 1/2(alpha)(dt^2) + (omega * dt) + thetaI

	TORQUE
		Torque is the measure of the force that causes an object
		to spin around an axis.

		dTorque
		Tau = distanceToOrigin x force

		applying at an angle, find perpendicular force
		//FINISH
		tau = Fr * sin(theta)

		F= ma for rotation:
			T = alpha * inertia

			inertia tensor component value for the axis of rotation

		get angular acceleration to add around a specifc axis using
			
			T/I = alpha
			positive torque = counterclockwise
			negative torque = clockwise

			J/I + omegaI = omegaF
*/

