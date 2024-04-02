#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <World/Entity.h>
#include <World/World.h>
#include <World/Terrain/Terrain.h>
#include <World/MoveableTestEntity.h>

namespace Bennett
{
    void World::DeleteAllEntities()
    {
        for (auto itr = m_Entities.begin(); itr != m_Entities.end(); /*it++*/)
        {
            Entity* entity = itr->second;
            if (entity == nullptr)
            {
                ++itr;
            }
            else
            {
                delete itr->second;
                itr->second = nullptr;
                itr = m_Entities.erase(itr);
            }
        }

        m_Entities.clear();
    }

    World::World()
    {
        m_Entities = std::unordered_map<std::string, Entity*>();
        m_IsLoaded = false;
    }

    World::~World()
    {
        for (auto itr = m_Entities.begin(); itr != m_Entities.end(); /*it++*/)
        {
            Entity* entity = itr->second;
            if (entity == nullptr)
            {
                ++itr;
            }
            else
            {
                delete itr->second;
                itr->second = nullptr;
                itr = m_Entities.erase(itr);
            }
        }
    }

    Entity* World::CreateTerrain()
    {
        auto itr = m_Entities.find("Terrain");

        if (itr != m_Entities.end())
        {
            Log("World: Tried to create an entity with a name that already exists.", LOG_MINIMAL);
            DestroyEntity(itr->second);
        }

        Entity* terrain = Terrain::Create(8);
        m_Entities.insert(std::make_pair("Terrain", terrain));
        
        if (ENABLE_LOG_SPAWN_ENTITY_NOTICE)
        {
            Log("World: Created a terrain entity.", LOG_SAFE);
        }

        return terrain;
    }

    Entity* World::SpawnTestEntity(const std::string& name)
    {
        MoveableTestEntity* entity = nullptr;
        auto itr = m_Entities.find(name);

        if (itr == m_Entities.end())
        {
            entity = new MoveableTestEntity(name);
            m_Entities.insert(std::make_pair(name, entity));

            if (ENABLE_LOG_SPAWN_ENTITY_NOTICE)
            {
                Log("World: Created an entity called \"" + name + "\"", LOG_SAFE);
            }
        }
        else
        {
            Log("World: Tried to create an entity with a name that already exists.", LOG_MINIMAL);
        }

        return (Entity*)entity;
    }

    Entity* World::SpawnEntity(const std::string& name)
    {
        return SpawnEntity(name, glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    }

    void World::DestroyEntity(Entity* entity)
    {
        if (entity == nullptr)
        {
            Log("World: Tried to delete an entity with an invalid pointer", LOG_MINIMAL);
            return;
        }

        auto itr = m_Entities.find(entity->GetName());
        if (itr != m_Entities.end())
        {
            delete itr->second;
            itr->second = nullptr;
            m_Entities.erase(itr);
        }
        else
        {
            Log("World: Tried to delete an entity that does not seem to exist in the map.", LOG_MINIMAL);
        }
    }

    const bool World::IsLoaded() const
    {
        return m_IsLoaded;
    }

    Entity* World::SpawnEntity(const std::string& name, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation)
    {
        Entity* entity = nullptr;
        auto itr = m_Entities.find(name);

        if (itr == m_Entities.end())
        {
            entity = new Entity(name, scale, position, rotation);
            m_Entities.insert(std::make_pair(name, entity));

            if (ENABLE_LOG_SPAWN_ENTITY_NOTICE)
            {
                Log("World: Created an entity called \"" + name + "\"", LOG_SAFE);
            }
        }
        else
        {
            Log("World: Tried to create an entity with a name that already exists.", LOG_MINIMAL);
        }

        return entity;
    }

    Entity* World::GetEntity(const std::string& name)
    {
        Entity* entity = nullptr;
        auto itr = m_Entities.find(name);

        if (itr != m_Entities.end())
        {
            return itr->second;
        }

        return entity;
    }

    void World::Update(const float& deltaTime)
    {
        for (auto& entity : m_Entities)
        {
            entity.second->Update(deltaTime);
        }
    }

    void World::Render(const Renderer& renderer)
    {
        for (auto& entity : m_Entities)
        {
            entity.second->Render(renderer);
        }
    }
}