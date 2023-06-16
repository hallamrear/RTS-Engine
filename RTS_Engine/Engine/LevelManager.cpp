#include "BennettPCH.h"
#include "LevelManager.h"
#include "AssetManager.h"
#include "Entity.h"
#include "Model.h"
#include "tinyxml2.h"

namespace Bennett
{
    void LevelManager::LoadLevel(const std::string& levelName, World& world)
    {
        if (world.IsLoaded())
        {
            UnloadLevel(world);
        }

        tinyxml2::XMLDocument document;
        document.LoadFile(levelName.c_str());

        if (document.Error())
        {
            return;
        }
            
        std::string terrainName = "";
        terrainName = document.FirstChildElement("Level")->FirstChildElement("TerrainName")->GetText();
        if (terrainName != "")
        {
            Entity* floor = world.SpawnEntity("Terrain");
            floor->SetModel(AssetManager::GetModel(terrainName));
            
            Entity* altFloor = world.SpawnEntity("Terrain2");
            altFloor->SetModel(AssetManager::GetModel(terrainName));
            altFloor->Translate(glm::vec3(200.0f, 200.0f, 0.0f));
            altFloor->Rotate(glm::vec3(0.0f, -90.0f, 90.0f));
        }

        world.m_IsLoaded = true;
    }

    void LevelManager::UnloadLevel(World& world)
    {
        if (world.IsLoaded() == false)
            return;

        world.DeleteAllEntities();

        world.m_IsLoaded = false;
    }
}