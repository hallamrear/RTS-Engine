#include "BennettPCH.h"
#include "TerrainChunk.h"
#include "MarchingCubesTriangulationTable.h"
#include "ServiceLocator.h"

namespace Bennett
{
	using namespace TriangulationData;

	int TerrainChunk::s_TerrainIDCount = 0;
	
	TerrainChunk::TerrainChunk() : Entity("TerrainChunk_" + std::to_string(s_TerrainIDCount))
	{
		s_TerrainIDCount++;
	}

	TerrainChunk::~TerrainChunk()
	{

	}

	void TerrainChunk::BuildChunk()
	{
		std::vector<Vertex> triangleVertices;
		std::vector<unsigned int> indices;

		glm::vec3 position = GetPosition();
		int index = 0;
		Vertex v1, v2, v3;
		glm::vec3 normal;

		for (size_t i = 0; i < GRID_RESOLUTION * GRID_RESOLUTION; i++)
		{
			int x = (i / GRID_RESOLUTION);
			int z = (i % GRID_RESOLUTION);

			v1 = Vertex(glm::vec3(position.x + x + Vertices[0].Position.x, position.y, position.z + z + Vertices[0].Position.z));
			v2 = Vertex(glm::vec3(position.x + x + Vertices[1].Position.x, position.y, position.z + z + Vertices[1].Position.z));
			v3 = Vertex(glm::vec3(position.x + x + Vertices[2].Position.x, position.y, position.z + z + Vertices[2].Position.z));
			normal = glm::cross(v2.Position - v1.Position, v3.Position - v1.Position);
			v1.Normal = normal; v2.Normal = normal; v3.Normal = normal;
			triangleVertices.push_back(v1); triangleVertices.push_back(v2); triangleVertices.push_back(v3);
			index = indices.size();
			indices.push_back(index + 0);
			indices.push_back(index + 1);
			indices.push_back(index + 2);

			v1 = Vertex(glm::vec3(position.x + x + Vertices[0].Position.x, position.y, position.z + z + Vertices[0].Position.z));
			v2 = Vertex(glm::vec3(position.x + x + Vertices[2].Position.x, position.y, position.z + z + Vertices[2].Position.z));
			v3 = Vertex(glm::vec3(position.x + x + Vertices[3].Position.x, position.y, position.z + z + Vertices[3].Position.z));
			normal = glm::cross(v2.Position - v1.Position, v3.Position - v1.Position);
			v1.Normal = normal; v2.Normal = normal; v3.Normal = normal;
			triangleVertices.push_back(v1); triangleVertices.push_back(v2); triangleVertices.push_back(v3);
			index = indices.size();
			indices.push_back(index + 0);
			indices.push_back(index + 1);
			indices.push_back(index + 2);
		}

		if (triangleVertices.size() != 0)
		{
			Model* chunkModel = ServiceLocator::GetAssetManager().CreateModel(GetName(), triangleVertices, indices);
			SetModel(chunkModel);
		}
	}


}