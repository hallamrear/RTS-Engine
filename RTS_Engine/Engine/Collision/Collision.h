#pragma once
#include <Collision/Collider/AABBCollider.h>
#include <Collision/Collider/SphereCollider.h>
#include <Collision/Collider/Ray.h>
#include <glm/fwd.hpp>
#include <System/Logger.h>

namespace Bennett
{
	class OBBCollider;

	class Collision
	{
	private:

	#pragma region Old Collision Functions
		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<GLM::VEC3, AABBCOLLIDER>()
		/// 
		/// Tests whether a point is inside an axis aligned bounding box.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="center">Centre point of the aabb</param>
		/// <param name="extents">The full width, height and depth of the aabb.</param>
		/// <returns>true if point is inside of the aabb. false if not.</returns>
		inline static bool PointInAABB(const glm::vec3& point, const glm::vec3& center, const glm::vec3& extents)
		{
			glm::vec3 halfExtents = extents / 2.0f;
			glm::vec3 min = center - halfExtents;
			glm::vec3 max = center + halfExtents;

			//Check if the point is less than max and greater than min
			if (point.x >= min.x && point.x < max.x &&
				point.y >= min.y && point.y < max.y &&
				point.z >= min.z && point.z < max.z)
			{
				return true;
			}

			//If not, then return false
			return false;
		};

		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<GLM::VEC3, AABBCOLLIDER>()
		/// 
		/// Tests whether a point is inside an axis aligned bounding box.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="aabb">Box to test if point is inside</param>
		/// <returns>true if point is inside of the aabb. false if not.</returns>
		inline static bool PointInAABB(const glm::vec3& point, const AABBCollider& aabb)
		{
			return PointInAABB(point, aabb.GetTransform().GetPosition(), aabb.GetExtents());
		};

		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<GLM::VEC3, SphereCollider>()
		/// 
		/// Tests whether a point is inside a sphere.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="sphere">Sphere used to test if point is inside</param>
		/// <returns>true if point is inside of the sphere. false if not.</returns>
		inline static bool PointInSphere(const glm::vec3& point, const glm::vec3& sphereCenter, const float& sphereRadius)
		{
			glm::vec3 direction = sphereCenter - point;
			//dot product of 2 identical vectors returns the square of the magnitude.
			float distanceSquared = glm::dot(direction, direction);
			float radiusSquared = sphereRadius * sphereRadius;
			//Avoiding using a sqrt and pythag by working in squares.
			return (distanceSquared < radiusSquared);
		};

		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<GLM::VEC3, SphereCollider>()
		/// 
		/// Tests whether a point is inside a sphere.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="sphere">SphereCollider used to test if point is inside</param>
		/// <returns>true if point is inside of the SphereCollider. false if not.</returns>
		inline static bool PointInSphere(const glm::vec3& point, const SphereCollider& sphere)
		{
			return PointInSphere(point, sphere.GetTransform().GetPosition(), sphere.GetRadius());
		};

		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<Ray, SphereCollider>()
		/// 
		/// Tests whether a ray intersects with a sphere.
		/// </summary>
		/// <param name="rayStart">Start of ray to test</param>
		/// <param name="rayDirection">Direction of ray to test</param>
		/// <param name="sphereCenter">Center of sphere to test</param>
		/// <param name="sphereRadius">Radius of sphere to test</param>
		/// <returns>Boolean value expressing whether ray intersects with sphere.</returns>
		inline static bool RayToSphere(const glm::vec3& rayStart, const glm::vec3& rayDirection, const glm::vec3& sphereCenter, const float& sphereRadius)
		{
			//If the ray starts inside the sphere, return early.
			if (PointInSphere(rayStart, sphereCenter, sphereRadius))
				return true;

			//Create a vector from the ray's start to the sphere's center
			glm::vec3  rayToSphereCenter = sphereCenter - rayStart;

			//Project this vector onto the ray's direction vector
			float projectedDistance = glm::dot(rayStart, rayDirection);

			//Ray is pointing away so return early.
			if (projectedDistance < 0.0f)
				return false;

			//Calculate the closest point to the sphere
			glm::vec3 closestPoint = (rayStart + (rayDirection * projectedDistance));

			//Check if that point is inside the sphere
			return (PointInSphere(closestPoint, sphereCenter, sphereRadius));
		};

		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<Ray, SphereCollider>()
		/// 
		/// Tests whether a ray intersects with a sphere.
		/// </summary>
		/// <param name="ray">Ray to test</param>
		/// <param name="sphere">Sphere collider to test</param>
		/// <returns>Boolean value expressing whether ray intersects with sphere.</returns>
		inline static bool RayToSphere(const Ray& ray, const SphereCollider& sphere)
		{
			return RayToSphere(ray.GetStart(), ray.GetDirection(), sphere.GetTransform().GetPosition(), sphere.GetRadius());
		};

		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<Ray, SphereCollider>()
		/// 
		/// Tests whether a ray intersects with a sphere.
		/// </summary>
		/// <param name="ray">Ray to test</param>
		/// <param name="sphereCenter">Center of sphere to test</param>
		/// <param name="sphereRadius">Radius of sphere to test</param>
		/// <returns>Boolean value expressing whether ray intersects with sphere.</returns>
		inline static bool RayToSphere(const Ray& ray, const glm::vec3& sphereCenter, const float& sphereRadius)
		{
			return RayToSphere(ray.GetStart(), ray.GetDirection(), sphereCenter, sphereRadius);
		};

		/// <summary>
		/// 
		/// SEE CHECKCOLLISION<Ray, SphereCollider>()
		/// 
		/// Tests whether a ray intersects with a sphere.
		/// </summary>
		/// <param name="rayStart">Start of ray to test</param>
		/// <param name="rayDirection">Direction of ray to test</param>
		/// <param name="sphere">Sphere collider to test</param>
		/// <returns>Boolean value expressing whether ray intersects with sphere.</returns>
		inline static bool RayToSphere(const glm::vec3& rayStart, const glm::vec3& rayDirection, const SphereCollider& sphere)
		{
			return RayToSphere(rayStart, rayDirection, sphere.GetTransform().GetPosition(), sphere.GetRadius());
		};
#pragma endregion

	public:
		template<class A, class B>
		inline static bool CheckCollision(const A& colliderA, const B& colliderB)
		{
			Log("Check collision function of two colliders that are not supported yet. Returning false.", LOG_SERIOUS);
			return false;
		};	
		
		/// <summary>
		/// Tests whether a point is inside a sphere.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="sphere">Sphere used to test if point is inside</param>
		/// <returns>true if point is inside of the sphere. false if not.</returns>
		template<>
		inline static bool CheckCollision<glm::vec3, SphereCollider>(const glm::vec3& point, const SphereCollider& sphere)
		{
			return CheckCollision<SphereCollider, glm::vec3>(sphere, point);
		};

		/// <summary>
		/// Tests whether a point is inside a sphere.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="sphere">Sphere used to test if point is inside</param>
		/// <returns>true if point is inside of the sphere. false if not.</returns>
		template<>
		inline static bool CheckCollision<SphereCollider, glm::vec3>(const SphereCollider& sphere, const glm::vec3& point)
		{
			glm::vec3 direction = sphere.GetTransform().GetPosition() - point;
			//dot product of 2 identical vectors returns the square of the magnitude.
			float distanceSquared = glm::dot(direction, direction);
			float radiusSquared = sphere.GetRadius() * sphere.GetRadius();
			//Avoiding using a sqrt and pythag by working in squares.
			return (distanceSquared < radiusSquared);
		};

		/// <summary>
		/// Tests whether a ray intersects with a sphere.
		/// </summary>
		/// <param name="ray">Ray object to test for collision with SphereCollider</param>
		/// <param name="sphere">SphereCollider used in check for ray intersection</param>
		/// <returns>Boolean value expressing whether ray intersects with sphere.</returns>
		template<>
		inline static bool CheckCollision<Ray, SphereCollider>(const Ray& ray, const SphereCollider& sphere)
		{
			return CheckCollision<SphereCollider, Ray>(sphere, ray);
		};

		/// <summary>
		/// Tests whether a ray intersects with a sphere.
		/// </summary>
		/// <param name="ray">Ray object to test for collision with SphereCollider</param>
		/// <param name="sphere">SphereCollider used in check for ray intersection</param>
		/// <returns>Boolean value expressing whether ray intersects with sphere.</returns>
		template<>
		inline static bool CheckCollision<SphereCollider, Ray>(const SphereCollider& sphere, const Ray& ray)
		{
			//If the ray starts inside the sphere, return early.
			if (CheckCollision<SphereCollider, glm::vec3>(sphere, ray.GetStart()))
				return true;

			//Create a vector from the ray's start to the sphere's center
			glm::vec3  rayToSphereCenter = sphere.GetTransform().GetPosition() - ray.GetStart();

			//Project this vector onto the ray's direction vector
			float projectedDistance = glm::dot(ray.GetStart(), ray.GetDirection());

			//Ray is pointing away so return early.
			if (projectedDistance < 0.0f)
				return false;

			//Calculate the closest point to the sphere
			glm::vec3 closestPoint = (ray.GetStart() + (ray.GetDirection() * projectedDistance));

			//Check if that point is inside the sphere
			return (CheckCollision<glm::vec3, SphereCollider>(closestPoint, sphere));
		};

		/// <summary>
		/// Tests whether an AABB intersects with a sphere.
		/// </summary>
		/// <param name="sphere">Sphere to test.</param>
		/// <param name="aabb">AABB to test.</param>
		/// <returns></returns>
		template<>
		inline static bool CheckCollision<SphereCollider, AABBCollider>(const SphereCollider& sphere, const AABBCollider& aabb)
		{
			const glm::vec3 spherePosition = sphere.GetTransform().GetPosition();
			glm::vec3 boxMin = aabb.GetMinBounds();
			glm::vec3 boxMax = aabb.GetMaxBounds();

			// get box closest point to sphere center by clamping
			const float x = std::max(boxMin.x, std::min(spherePosition.x, boxMax.x));
			const float y = std::max(boxMin.y, std::min(spherePosition.y, boxMax.y));
			const float z = std::max(boxMin.z, std::min(spherePosition.z, boxMax.z));

			return CheckCollision<glm::vec3, SphereCollider>(glm::vec3(x, y, z), sphere);
		};

		/// <summary>
		/// Tests whether an intersection has occured between a sphere and an oriented bounding box.
		/// </summary>
		/// <param name="sphere">Sphere to test.</param>
		/// <param name="obb">Oriented bounding box to test.</param>
		/// <returns></returns>
		template<>
		inline static bool CheckCollision<SphereCollider, OBBCollider>(const SphereCollider& sphere, const OBBCollider& obb)
		{
			//TODO : Implement
			throw;
			return false;
		};

		/// <summary>
		/// Checks for an intersection between 2 sphere colliders.
		/// </summary>
		/// <param name="colliderA">One of the colliders involved in the check.</param>
		/// <param name="colliderB">The other collider involved in the check.</param>
		/// <returns></returns>
		template<>
		inline static bool CheckCollision<SphereCollider, SphereCollider>(const SphereCollider& colliderA, const SphereCollider& colliderB)
		{
			const glm::vec3 positionDiff = colliderA.GetTransform().GetPosition() - colliderB.GetTransform().GetPosition();

			const float distanceSquared = 
			   ((positionDiff.x) * (positionDiff.x) +
				(positionDiff.y) * (positionDiff.y) +
				(positionDiff.z) * (positionDiff.z));

			const float radiusSum = colliderA.GetRadius() + colliderB.GetRadius();
			const float radiusSquared = radiusSum * radiusSum;

			return distanceSquared <= radiusSquared; 
		};

		/// <summary>
		/// Tests whether a point is inside an axis aligned bounding box.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="aabb">Axis aligned bounding box that is tested against.</param>
		/// <returns>true if point is inside of the aabb. false if not.</returns>
		template<>
		inline static bool CheckCollision<glm::vec3, AABBCollider>(const glm::vec3& point, const AABBCollider& aabb)
		{
			return CheckCollision<AABBCollider, glm::vec3>(aabb, point);
		};

		/// <summary>
		/// Tests whether a point is inside an axis aligned bounding box.
		/// </summary>
		/// <param name="point">Point to test</param>
		/// <param name="aabb">Axis aligned bounding box that is tested against.</param>
		/// <returns>true if point is inside of the aabb. false if not.</returns>
		template<>
		inline static bool CheckCollision<AABBCollider, glm::vec3>(const AABBCollider& aabb, const glm::vec3& point)
		{	
			glm::vec3 halfExtents = aabb.GetExtents() / 2.0f;
			glm::vec3 min = aabb.GetTransform().GetPosition() - halfExtents;
			glm::vec3 max = aabb.GetTransform().GetPosition() + halfExtents;

			//Check if the point is less than max and greater than min
			if (point.x >= min.x && point.x < max.x &&
				point.y >= min.y && point.y < max.y &&
				point.z >= min.z && point.z < max.z)
			{
				return true;
			}

			//If not, then return false
			return false;
		};

		template<>
		inline static bool CheckCollision<Ray, AABBCollider>(const Ray& ray, const AABBCollider& aabb)
		{
			return CheckCollision<AABBCollider, Ray>(aabb, ray);
		};

		template<>
		inline static bool CheckCollision<AABBCollider, Ray>(const AABBCollider& aabb, const Ray& ray)
		{
			//TODO : Implement
			throw;
			return false;
		};

		template<>													
		inline static bool CheckCollision<AABBCollider, AABBCollider>(const AABBCollider& colliderA, const AABBCollider& colliderB)
		{
			glm::vec3 colliderAMax = colliderA.GetMaxBounds();
			glm::vec3 colliderAMin = colliderA.GetMinBounds();

			glm::vec3 colliderBMax = colliderB.GetMaxBounds();
			glm::vec3 colliderBMin = colliderB.GetMinBounds();

			return (
				colliderAMin.x <= colliderBMax.x &&
				colliderAMax.x >= colliderBMin.x &&
				colliderAMin.y <= colliderBMax.y &&
				colliderAMax.y >= colliderBMin.y &&
				colliderAMin.z <= colliderBMax.z &&
				colliderAMax.z >= colliderBMin.z
				);
		};

		template<>
		inline static bool CheckCollision<AABBCollider, OBBCollider>(const AABBCollider& aabb, const OBBCollider& obb)
		{
			//TODO : Implement
			throw;
			return false;
		};

		/// <summary>
		/// Tests whether an AABB intersects with a sphere.
		/// </summary>
		/// <param name="sphere">Sphere to test.</param>
		/// <param name="aabb">AABB to test.</param>
		/// <returns></returns>
		template<>
		inline static bool CheckCollision<AABBCollider, SphereCollider>(const AABBCollider& aabb, const SphereCollider& sphere)
		{
			return CheckCollision<SphereCollider, AABBCollider>(sphere, aabb);
		};

		template<>
		inline static bool CheckCollision<glm::vec3, OBBCollider>(const glm::vec3& point, const OBBCollider& obb)
		{
			return CheckCollision<OBBCollider, glm::vec3>(obb, point);
		};

		template<>
		inline static bool CheckCollision<OBBCollider, glm::vec3>(const OBBCollider& obb, const glm::vec3& point)
		{
			//TODO : Implement
			throw;
			return false;
		};

		template<>
		inline static bool CheckCollision<Ray, OBBCollider>(const Ray& ray, const OBBCollider& obb)
		{
			return CheckCollision<OBBCollider, Ray>(obb, ray);
		};

		template<>
		inline static bool CheckCollision<OBBCollider, Ray>(const OBBCollider& obb, const Ray& colliderB)
		{
			//TODO : Implement
			throw;
			return false;
		};

		template<>
		inline static bool CheckCollision<OBBCollider, AABBCollider>(const OBBCollider& obb, const AABBCollider& aabb)
		{
			return CheckCollision<AABBCollider, OBBCollider>(aabb, obb);
		};

		template<>
		inline static bool CheckCollision<OBBCollider, OBBCollider>(const OBBCollider& colliderA, const OBBCollider& colliderB)
		{
			//TODO : Implement
			throw;
			return false;
		};

		template<>
		inline static bool CheckCollision<OBBCollider, SphereCollider>(const OBBCollider& obb, const SphereCollider& sphere)
		{
			return CheckCollision<SphereCollider, OBBCollider>(sphere, obb);
		};
	};

}