#pragma once
#include <Collision/Collider/Collider.h>

namespace Bennett
{
	struct BENNETT_ENGINE CollisionDetails2D
	{
		float Depth;
		glm::vec2 Normal;
	};

	struct BENNETT_ENGINE CollisionPoint
	{
		const Collider* Subject = nullptr;
		glm::vec3 HitPoint = glm::vec3(0.0f);;
	};

	struct BENNETT_ENGINE CollisionDetails
	{
		float Depth = 0.0f;
		glm::vec3 Normal = glm::vec3(0.0f);
		glm::vec3 Tangent = glm::vec3(0.0f);

		CollisionPoint CollisionPoints[2];
	};

	template<class T>
	struct BENNETT_ENGINE SupportVertex_t
	{
		T SupportVertexA;
		T SupportVertexB;
		T MinkowskiDifference;

		SupportVertex_t(const T& supportA, const T& supportB)
			: SupportVertexA(supportA), SupportVertexB(supportB), MinkowskiDifference(SupportVertexA - SupportVertexB)
		{

		}

		SupportVertex_t(const SupportVertex_t& supportVertex)
			: SupportVertexA(supportVertex.SupportVertexA), SupportVertexB(supportVertex.SupportVertexB), MinkowskiDifference(supportVertex.MinkowskiDifference)
		{

		}

		SupportVertex_t()
		{
			SupportVertexA = T();
			SupportVertexB = T();
			MinkowskiDifference = T();
		}
	};

	typedef SupportVertex_t<glm::vec3> SupportVertex;
	typedef SupportVertex_t<glm::vec2> SupportVertex2D;

	class EPA_Result
	{
	public:
		struct Triangle
		{
			SupportVertex points[3];
			glm::vec3 normal;

			Triangle()
			{
				points[0] = { SupportVertex() };
				points[1] = { SupportVertex() };
				points[2] = { SupportVertex() };
				normal = { glm::vec3(0.0f) };
			}
		} tri;

		SupportVertex supportVertex;
	};
};