#include "BennettPCH.h"
#include "GridBasedTerrain.h"

namespace Bennett
{
	GridBasedTerrain::GridBasedTerrain() : Entity("GridBasedTerrain")
	{
		m_Texture = nullptr;
	}

	void GridBasedTerrain::LoadTerrainDataFromFile(const std::string& filePath)
	{
		//todo : implement properly.





	}

	void GridBasedTerrain::GenerateMesh(const std::string& filePath)
	{
		LoadTerrainDataFromFile(filePath);
	}

	GridBasedTerrain::~GridBasedTerrain()
	{

	}

	GridBasedTerrain* GridBasedTerrain::Create(const std::string& filePath)
	{
		GridBasedTerrain* terrain = new GridBasedTerrain();
		terrain->GenerateMesh(filePath);
		return terrain;
	}

	void GridBasedTerrain::Render(const Renderer& renderer)
	{
	}
}