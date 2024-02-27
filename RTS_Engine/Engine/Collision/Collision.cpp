#include <BennettPCH.h>
#include <Collision/Collision.h>
#include <Defines/HelperFunctions.h>

using namespace Bennett::Helper;

namespace Bennett
{
	bool GJK::Line(std::vector<glm::vec3>& simplex, glm::vec3& direction)
	{
		glm::vec3 a = simplex[0];
		glm::vec3 b = simplex[1];

		glm::vec3 ab = b - a;
		glm::vec3 ao = -a;

		if (SameDirection(ab, ao))
		{
			direction = glm::cross(glm::cross(ab, ao), ab);
		}
		else
		{
			simplex.clear();
			simplex = { a };
			direction = ao;
		}

		return false;
	};

	bool GJK::Triangle(std::vector<glm::vec3>& simplex, glm::vec3& direction)
	{
		glm::vec3 a = simplex[0];
		glm::vec3 b = simplex[1];
		glm::vec3 c = simplex[2];

		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;
		glm::vec3 ao = -a;

		glm::vec3 abc = cross(ab, ac);

		if (SameDirection(glm::cross(abc, ac), ao)) 
		{
			if (SameDirection(ac, ao))
			{
				simplex.clear();
				simplex = { a, c };
				direction = cross(cross(ac, ao), ac);
			}
			else
			{
				simplex.clear();
				simplex = { a, b };
				return Line(simplex, direction);
			}
		}
		else 
		{
			if (SameDirection(cross(ab, abc), ao)) 
			{
				simplex.clear();
				simplex = { a, b };
				return Line(simplex, direction);
			}
			else
			{
				if (SameDirection(abc, ao))
				{
					direction = abc;
				}
				else
				{
					simplex.clear();
					simplex = { a, c, b };
					direction = -abc;
				}
			}
		}

		return false;
	};

	bool GJK::Tetrahedron(std::vector<glm::vec3>& simplex, glm::vec3& direction)
	{
		glm::vec3 a = simplex[0];
		glm::vec3 b = simplex[1];
		glm::vec3 c = simplex[2];
		glm::vec3 d = simplex[3];

		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;
		glm::vec3 ad = d - a;
		glm::vec3 ao = -a;

		//Construct new edges for top point of tetrahedron
		glm::vec3 abc = cross(ab, ac);
		glm::vec3 acd = cross(ac, ad);
		glm::vec3 adb = cross(ad, ab);

		//Check each triangle.
		if (SameDirection(abc, ao)) 
		{
			simplex.clear();
			simplex = { a, b, c };
			return Triangle(simplex, direction);
		}

		if (SameDirection(acd, ao))
		{
			simplex.clear();
			simplex = { a, c, d };
			return Triangle(simplex, direction);
		}

		if (SameDirection(adb, ao)) 
		{
			simplex.clear();
			simplex = { a, d, b };
			return Triangle(simplex, direction);
		}

		return true;
	};

	bool GJK::UpdateSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction)
	{
		switch (simplex.size())
		{
			//Simplex is line AB
			case 2: { return Line(simplex, direction); } break;

			//Simplex is triangle ABC
			case 3: { return Triangle(simplex, direction); } break;

			//Simplex is formed of 3 triangles, ABC, ABD, ACD, BCD
			case 4: { return Tetrahedron(simplex, direction); } break;

		default:
			Log(LOG_SERIOUS, "GJK Simplex updating should not have reached here.\n");
			return false;
			break;
		}
	}

	bool GJK::CheckCollision(const Collider& colliderA, const Collider& colliderB, CollisionDetails* manifold)
	{
		glm::vec3 direction = (colliderA.GetTransform().GetPosition() + colliderA.GetOffset()) +
							  (colliderB.GetTransform().GetPosition() + colliderB.GetOffset());

		//Get support vertex in initial direction.
		direction = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 support = Helper::Collision::GetSupportVertex(colliderA, colliderB, direction);

		std::vector<glm::vec3> simplex;
		simplex = { support };

		//Get a new direction in the opposite direction for the first loop.
		direction = -support;

		while (true)
		{
			support = Helper::Collision::GetSupportVertex(colliderA, colliderB, direction);

			//returns if the new point is not in front of the search direction
			//this would exit if the direction finds a vertex that was already the furthest one along it.
			if (glm::dot(support, direction) <= 0)
			{
				return false;
			}

			//Add new support vertex to simplex.
			simplex.insert(simplex.begin(), support);

			//If detection in simplex, return true.
			if (UpdateSimplex(simplex, direction))
			{
				if (manifold != nullptr)
				{
					EPA::GetCollisionDetails(simplex, colliderA, colliderB, manifold);
				}

				return true;
			}
		}

		return false;
	}

	void Collision::GetNormalAxesOfBoundingBox(const Collider& collider, std::array<glm::vec3, 6>& axes)
	{
		static std::array<glm::vec3, 3> sBasisVectors =
		{
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		axes =
		{
			+sBasisVectors[0],
			+sBasisVectors[1],
			+sBasisVectors[2],
			-sBasisVectors[0],
			-sBasisVectors[1],
			-sBasisVectors[2]
		};

		glm::mat4 rotationMatrix{ 1.0f };
		glm::vec4 pos{ 0.0f };
		if (collider.GetType() == ColliderType::OBB)
		{
			rotationMatrix = glm::toMat4(collider.GetTransform().GetRotation());
			for (size_t i = 0; i < 6; i++)
			{
				//is .xyz,0 as it is an axis vector.
				pos = glm::vec4(axes[i], 0.0f);
				axes[i] = pos * rotationMatrix;
			}
		}
	};

	void EPA::GetCollisionDetails(const std::vector<glm::vec3>& simplex, const Collider& colliderA, const Collider& colliderB, CollisionDetails* manifold)
	{
		if (manifold == nullptr)
		{
			Log(LOG_SERIOUS, "Called EPA::GetCollisionDetails with a nullptr manifold.\n");
			return;
		}


		if (simplex.size() < 4)
		{
			Log(LOG_SERIOUS, "Called EPA::GetCollisionDetails with a simplex containing less than a 3d object.\n");
			return;
		}


		manifold->Normal = glm::vec3();
		manifold->Depth = 0.001f;
	}
}
