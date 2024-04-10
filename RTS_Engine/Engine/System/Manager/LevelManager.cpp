#include <BennettPCH.h>
#include <System/Manager/LevelManager.h>
#include <System/Manager/AssetManager.h>
#include <World/Entity.h>
#include <World/World.h>
#include <Rendering/Model.h>
#include <External/tinyxml2.h>
#include <System/ServiceLocator.h>

namespace Bennett
{
    void LevelManager::LoadLevel(const std::string& levelName, World& world)
    {
        if (world.IsLoaded())
        {
            UnloadLevel(world);
        }

        tinyxml2::XMLDocument document;
        std::string fileLocation = ServiceLocator::GetResourceFolderLocation() + levelName.c_str();
        document.LoadFile(fileLocation.c_str());

        if (document.Error())
        {
            return;
        }
            
        std::string terrainName = "";
        terrainName = document.FirstChildElement("Level")->FirstChildElement("TerrainName")->GetText();
        if (terrainName != "")
        {
            Entity* floor = world.SpawnEntity("tank");
            floor->SetModel(AssetManager::GetModel("tank"));
            if(floor->HasModel())
                floor->SetTexture(AssetManager::GetTexture("tank"));

            floor->GetTransform().SetPosition(glm::vec3(5.0f, 0.0f, 5.0f));
            floor->GetTransform().SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
        }

        Terrain* Terrain = world.CreateTerrain();

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