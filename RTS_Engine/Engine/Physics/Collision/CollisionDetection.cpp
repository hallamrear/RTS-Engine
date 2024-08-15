#include <BennettPCH.h>
#include <Physics/Collision/CollisionDetection.h>
#include <Defines/HelperFunctions.h>

using namespace Bennett::Helper;

namespace Bennett
{
	bool GJK::Line(std::vector<SupportVertex>& simplex, glm::vec3& direction)
	{
		SupportVertex a = simplex[0];
		SupportVertex b = simplex[1];

		glm::vec3 ab = b.MinkowskiDifference - a.MinkowskiDifference;
		glm::vec3 ao = -a.MinkowskiDifference;

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

	bool GJK::Triangle(std::vector<SupportVertex>& simplex, glm::vec3& direction)
	{
		SupportVertex a = simplex[0];
		SupportVertex b = simplex[1];
		SupportVertex c = simplex[2];

		glm::vec3 ab = b.MinkowskiDifference - a.MinkowskiDifference;
		glm::vec3 ac = c.MinkowskiDifference - a.MinkowskiDifference;
		glm::vec3 ao = -a.MinkowskiDifference;

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

	bool GJK::Tetrahedron(std::vector<SupportVertex>& simplex, glm::vec3& direction)
	{
		SupportVertex a = simplex[0];
		SupportVertex b = simplex[1];
		SupportVertex c = simplex[2];
		SupportVertex d = simplex[3];

		glm::vec3 ab = b.MinkowskiDifference - a.MinkowskiDifference;
		glm::vec3 ac = c.MinkowskiDifference - a.MinkowskiDifference;
		glm::vec3 ad = d.MinkowskiDifference - a.MinkowskiDifference;
		glm::vec3 ao = -a.MinkowskiDifference;

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

	bool GJK::UpdateSimplex(std::vector<SupportVertex>& simplex, glm::vec3& direction)
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
		SupportVertex support = Collision::GetSupportVertex(colliderA, colliderB, direction);

		std::vector<SupportVertex> simplex;
		simplex = { support };

		//Get a new direction in the opposite direction for the first loop.
		direction = -support.MinkowskiDifference;

		while (true)
		{
			support = Collision::GetSupportVertex(colliderA, colliderB, direction);

			//returns if the new point is not in front of the search direction
			//this would exit if the direction finds a vertex that was already the furthest one along it.
			if (glm::dot(support.MinkowskiDifference, direction) <= 0)
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
					EPA_Result CSO = EPA::GetCollisionDetails(simplex, colliderA, colliderB, manifold);

					////This is our closest point to the origin on the CSO’s boundary.
					glm::vec3 PointOnTri = (manifold->Normal * manifold->Depth);
					//
					//Compute the barycentric coordinates of this closest point in the CSO triangle.
					glm::vec3 BaryCentPointOnTri = CalculateBarycentricPositionOnTriangle(PointOnTri, CSO.tri.points[0].MinkowskiDifference, CSO.tri.points[1].MinkowskiDifference, CSO.tri.points[2].MinkowskiDifference);
				
					//Linearly combining the original points with the same barycentric coordinates as coefficients
					//Ap = x Aa + y Ab + z Ac
					//Bp = x Ba + y Bb + z Bc
					glm::vec3 HitPointA = (BaryCentPointOnTri.x * CSO.tri.points[0].SupportVertexA) + (BaryCentPointOnTri.y * CSO.tri.points[1].SupportVertexA) + (BaryCentPointOnTri.z * CSO.tri.points[2].SupportVertexA);
					glm::vec3 HitPointB = (BaryCentPointOnTri.x * CSO.tri.points[0].SupportVertexB) + (BaryCentPointOnTri.y * CSO.tri.points[1].SupportVertexB) + (BaryCentPointOnTri.z * CSO.tri.points[2].SupportVertexB);

					manifold->CollisionPoints[0].HitPoint = HitPointA;
					manifold->CollisionPoints[0].Subject = &colliderA;
					manifold->CollisionPoints[1].HitPoint = HitPointB;
					manifold->CollisionPoints[1].Subject = &colliderB;

					if (ENABLE_LOG_COLLISION_MANIFOLD_PRINT)
					{
						printf(
							"Manifold details:\n\tNormal: {%f, %f, %f}\n\tDepth: %f\n\tHit Point A: {% f, % f, % f}\n\tHit Point B: {% f, % f, % f}\n\n",
							manifold->Normal.x,
							manifold->Normal.y,
							manifold->Normal.z,
							manifold->Depth,
							HitPointA.x, HitPointA.y, HitPointA.z,
							HitPointB.x, HitPointB.y, HitPointB.z
						);
					}
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

	int EPA::GetFaceNormals(std::vector<glm::vec3>& normals, std::vector<float>& distances, const std::vector<SupportVertex>& simplex, const std::vector<size_t>& faces)
	{
		int minIndex = 0;
		float minDistance = FLT_MAX;

		for (int i = 0; i < faces.size(); i += 3)
		{
			glm::vec3 a = simplex[faces[i + 0]].MinkowskiDifference;
			glm::vec3 b = simplex[faces[i + 1]].MinkowskiDifference;
			glm::vec3 c = simplex[faces[i + 2]].MinkowskiDifference;

			glm::vec3 lineAB = b - a;
			glm::vec3 lineAC = c - a;
			glm::vec3 normal = glm::normalize(glm::cross(lineAB, lineAC));
			
			//Project face line onto normal.
			float distance = glm::dot(normal, a);

			//Ensure normal is facing the right direction.
			if (distance < 0)
			{
				normal *= -1.0f;
				distance *= -1.0f;
			}

			normals.push_back(normal);
			distances.push_back(distance);

			if (distance < minDistance) 
			{
				//Index is the start of the triangle indices in 'faces'
				minIndex = i / 3;
				minDistance = distance;
			}

		}

		return minIndex;
	}

	void EPA::AddEdgeIfUnique(std::vector<std::pair<size_t, size_t>>& edgeList, const std::vector<size_t>& faceList, const size_t& indexA, const size_t& indexB)
	{
		auto edgeItr = std::find(edgeList.begin(), edgeList.end(), std::make_pair(faceList[indexB], faceList[indexA]));
		
		if (edgeItr != edgeList.end())
		{
			//if it does exist, get rid.
			edgeList.erase(edgeItr);
		}
		else
		{
			//if it doesnt, add it.
			edgeList.emplace_back(faceList[indexA], faceList[indexB]);
		}
	}

	EPA_Result EPA::GetCollisionDetails(std::vector<SupportVertex>& simplex, const Collider& colliderA, const Collider& colliderB, CollisionDetails* manifold)
	{
		EPA_Result result{};

		if (manifold == nullptr)
		{
			Log(LOG_SERIOUS, "Called EPA::GetCollisionDetails with a nullptr manifold.\n");
			return result;
		}

		if (simplex.size() < 4)
		{
			Log(LOG_SERIOUS, "Called EPA::GetCollisionDetails with a simplex containing less than a 3d object.\n");
			return result;
		}

		//Construct a new polytope (fancy word for 3d simplex) that we can add to without adjusting the original.
		std::vector<SupportVertex> polytope(simplex.begin(), simplex.end());

		int iterations = 0;

		std::vector<size_t> polytopeFaces = 
		{
			0, 1, 2,
			0, 3, 1,
			0, 2, 3,
			1, 3, 2
		};

		//Get and store the inital normals and distance values for each face.
		std::vector<glm::vec3> faceNormals = std::vector<glm::vec3>();
		std::vector<float> faceDistances = std::vector<float>();
		size_t minIndex = GetFaceNormals(faceNormals, faceDistances, simplex, polytopeFaces);

		float minDistance = FLT_MAX;
		glm::vec3 minNormal = glm::vec3();

		while (minDistance == FLT_MAX)
		{
			if (iterations > COLLISION_EPA_ITERATION_CAP)
				break;

			//Rather than generating a normal like in 2D EPA,
			//we get a normal from our pregenerated list.
			minNormal = faceNormals[minIndex];
			minDistance = faceDistances[minIndex];
			
			//Once we have the furthest edge normal, get the support vertex in that direction.
			SupportVertex supportVertex = Collision::GetSupportVertex(colliderA, colliderB, minNormal);
			//if this point is further out in the direction, we insert this vertex into the simplex and test again.
			float supportDistance = glm::dot(minNormal, supportVertex.MinkowskiDifference);

			if (abs(supportDistance - minDistance) > 0.001f)
			{
				minDistance = FLT_MAX;

				//Expanding polytrope in 3D does not just require adding a vertex
				//it needs to repair the faces. just adding a face however, 
				//may result in multiple identical support points.

				std::vector<std::pair<size_t, size_t>> uniqueEdgeList = std::vector<std::pair<size_t, size_t>>();

				for (size_t i = 0; i < faceNormals.size(); i++)
				{
					//Not just removing current face, but also every face pointing in that direciton.
					if (SameDirection(faceNormals[i], supportVertex.MinkowskiDifference))
					{
						size_t faceIndex = i * 3;

						//Add unique edges based on previous face.
						AddEdgeIfUnique(uniqueEdgeList, polytopeFaces, faceIndex + 0, faceIndex + 1);
						AddEdgeIfUnique(uniqueEdgeList, polytopeFaces, faceIndex + 1, faceIndex + 2);
						AddEdgeIfUnique(uniqueEdgeList, polytopeFaces, faceIndex + 2, faceIndex + 0);

						//Erase face and normals.
						polytopeFaces[faceIndex + 2] = polytopeFaces.back(); polytopeFaces.pop_back();
						polytopeFaces[faceIndex + 1] = polytopeFaces.back(); polytopeFaces.pop_back();
						polytopeFaces[faceIndex + 0] = polytopeFaces.back(); polytopeFaces.pop_back();

						faceNormals[i] = faceNormals.back(); // pop-erase
						faceNormals.pop_back();

						faceDistances[i] = faceDistances.back();
						faceDistances.pop_back();

						i--;
					}
				}

				//Add the new support point to the polytope 
				//and keep track of teh new faces from edges.
				std::vector<size_t> newFaces = std::vector<size_t>();
				for (std::pair<int, int> edge : uniqueEdgeList)
				{
					newFaces.push_back(edge.first);
					newFaces.push_back(edge.second);
					newFaces.push_back(polytope.size());
				}

				polytope.push_back(supportVertex);

				std::vector<glm::vec3> newNormals = std::vector<glm::vec3>();
				std::vector<float> newDistances = std::vector<float>();
				int newMinIndex = GetFaceNormals(newNormals, newDistances, polytope, newFaces);

				//Afer getting all the normals, check if that is the new closest face.
				//by comparing the new faceNormals against the originals.
				float oldDistance = FLT_MAX;

				for (size_t i = 0; i < faceDistances.size(); i++)
				{
					if (faceDistances[i] < oldDistance)
					{
						oldDistance = faceDistances[i];
						minIndex = i;
					}
				}

				if (newDistances[newMinIndex] < oldDistance)
				{
					minIndex = newMinIndex + faceDistances.size();
				}

				//Add new faces.
				polytopeFaces.insert(polytopeFaces.end(), newFaces.begin(), newFaces.end());
				//Add new normals.
				faceNormals.insert(faceNormals.end(), newNormals.begin(), newNormals.end());
				//Add new distances.
				faceDistances.insert(faceDistances.end(), newDistances.begin(), newDistances.end());

				iterations++;
			}
		}

		//Return the normal and depth (add a tiny amount to avoid multiple collisions).
		manifold->Normal = glm::normalize(minNormal);
		manifold->Depth = minDistance + 0.001f;

		EPA_Result::Triangle triangle;
		for (size_t i = 0; i < 3; i++)
		{
			size_t triFaceIndex = (minIndex * 3) + i;
			triangle.points[i] = (polytope[polytopeFaces[triFaceIndex]]);
		}

		glm::vec3 norm = glm::cross(triangle.points[1].MinkowskiDifference - triangle.points[0].MinkowskiDifference, triangle.points[2].MinkowskiDifference - triangle.points[0].MinkowskiDifference);
		norm = glm::normalize(norm);

		triangle.normal = Helper::GetNormalOfTriangle(triangle.points[0].MinkowskiDifference, triangle.points[1].MinkowskiDifference, triangle.points[2].MinkowskiDifference);

		result.tri = triangle;

		return result;
	}

	void EPA::Get2DCollisionDetails(std::vector<glm::vec2>& simplex, const Collider2D& colliderA, const Collider2D& colliderB, CollisionDetails2D* manifold)
	{
		if (manifold == nullptr)
		{
			Log(LOG_SERIOUS, "Called EPA::GetCollisionDetails with a nullptr manifold.\n");
			return;
		}

		if (simplex.size() < 3)
		{
			Log(LOG_SERIOUS, "Called EPA::GetCollisionDetails with a simplex containing less than a triangle.\n");
			return;
		}

		int minIndex = 0;
		float minDistance = FLT_MAX;
		glm::vec2 minNormal = glm::vec2();

		while (minDistance == FLT_MAX)
		{
			//Find the edge closest to the origin.
			//Comparing dot products between the edge normals and the vectors to the origin.
			//Store the closest one, alongside its distance and index.
			for (size_t i = 0; i < simplex.size(); i++)
			{
				//Create an edge from verts i & j.
				int j = (i + 1) % simplex.size();
				glm::vec2 vertexA = simplex[i];
				glm::vec2 vertexB = simplex[j];
				glm::vec2 lineAB = (vertexB - vertexA);

				//Create normal to the line.
				glm::vec2 abNorm = glm::vec2(lineAB.y, -lineAB.x);
				abNorm = glm::normalize(abNorm);
				//Project line onto vector to origin.
				float distance = glm::dot(abNorm, vertexA);

				//Ensure its in the right direction
				//(given left/right handedness of vertex order it could be facing outward)
				if (distance < 0)
				{
					distance *= -1;
					abNorm *= -1.0f;
				}

				if (distance < minDistance)
				{
					minDistance = distance;
					minNormal = abNorm;
					minIndex = j;
				}
			}

			//Once we have the furthest edge normal, get the support vertex in that direction.
			SupportVertex2D supportVertex = Collision::GetSupportVertex2D(colliderA, colliderB, minNormal);
			//if this point is further out in the direction, we insert this vertex into the simplex and test again.
			float supportDistance = glm::dot(minNormal, supportVertex.MinkowskiDifference);

			if (abs(supportDistance - minDistance) > 0.001f)
			{
				minDistance = FLT_MAX;
				simplex.insert(simplex.begin() + minIndex, supportVertex.MinkowskiDifference);
			}

			//if this point is not further out, the furthest has been found and it can be returned.
			manifold->Depth = minDistance + 0.001f;
			manifold->Normal = glm::vec2(minNormal);
		}
	}
}
