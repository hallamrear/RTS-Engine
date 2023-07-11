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
            Entity* floor = world.SpawnEntity("tools");
            floor->SetModel(AssetManager::GetModel("tools"));
            floor->SetTexture(AssetManager::GetTexture("tools"));
        }

        Entity* Terrain = world.CreateTerrain();

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