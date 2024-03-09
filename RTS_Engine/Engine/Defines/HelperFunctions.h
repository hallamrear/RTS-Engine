#pragma once

namespace Bennett
{
	namespace Helper
	{
		inline static glm::vec3 GetNormalOfTriangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C)
		{
			glm::vec3 normal = glm::vec3(0.0f);
			normal = glm::normalize(glm::cross(B - A, C - A));
			return normal;
		}

		//Compute barycentric coordinates for pointOnTri with respect to triangle (a, b, c)
		//Calculation implementation taken from 'Real Time Collision Detection' by Christer Ericson.
		inline static glm::vec3 CalculateBarycentricPositionOnTriangle(const glm::vec3& pointOnTri, const glm::vec3& triA, const glm::vec3& triB, const glm::vec3& triC)
		{
			glm::vec3 uvw = glm::vec3(0.0f);
			glm::vec3 v0 = triB - triA, v1 = triC - triA, v2 = pointOnTri - triA;
			float d00 = glm::dot(v0, v0);
			float d01 = glm::dot(v0, v1);
			float d11 = glm::dot(v1, v1);
			float d20 = glm::dot(v2, v0);
			float d21 = glm::dot(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			uvw.y = (d11 * d20 - d01 * d21) / denom;
			uvw.z = (d00 * d21 - d01 * d20) / denom;
			uvw.x = 1.0f - uvw.y - uvw.z;
			return uvw;
		}

		inline static bool SameDirection(const glm::vec3& direction, const glm::vec3& ao)
		{
			return glm::dot(direction, ao) > 0;
		};
	}
}