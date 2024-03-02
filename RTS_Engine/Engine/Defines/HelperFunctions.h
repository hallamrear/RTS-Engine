#pragma once

namespace Bennett
{
	namespace Helper
	{
		namespace Collision
		{
			inline static glm::vec3 GetSupportVertex(const Collider& colliderA, const Collider& colliderB, const glm::vec3& direction)
			{
				glm::vec3 dirNorm = glm::normalize(direction);
				glm::vec3 invDirNorm = dirNorm * -1.0f;
				return (colliderA.GetSupportVertex(direction) - colliderB.GetSupportVertex(invDirNorm));
			};

			inline static glm::vec2 GetSupportVertex2D(const Collider2D& colliderA, const Collider2D& colliderB, const glm::vec2& direction)
			{
				glm::vec3 dirNorm = glm::vec3(direction, 0.0f);
				glm::vec3 invDirNorm = dirNorm * -1.0f;
				return (colliderA.GetSupportVertex(dirNorm) - colliderB.GetSupportVertex(invDirNorm));
			};

		}

		inline static bool SameDirection(const glm::vec3& direction, const glm::vec3& ao)
		{
			return glm::dot(direction, ao) > 0;
		};
	}
}