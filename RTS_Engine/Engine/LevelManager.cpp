#include "BennettPCH.h"
#include "LevelManager.h"
#include "AssetManager.h"
#include "Entity.h"
#include "tinyxml2.h"

namespace Bennett
{
    void LevelManager::LoadLevel(const Renderer& renderer, const std::string& levelName, World& world)
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
            floor->_Model = AssetManager::GetModel(renderer, terrainName);
        }
    }

    void LevelManager::UnloadLevel(World& world)
    {
        if (world.IsLoaded() == false)
            return;


    }
}