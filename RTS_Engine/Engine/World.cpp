#include "BennettPCH.h"
#include "Renderer.h"
#include "World.h"
#include "Entity.h"

namespace Bennett
{
    void World::DeleteAllEntities()
    {
        for (auto& ent : m_Entities)
        {
            DestroyEntity(ent.second);
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

    Entity* World::SpawnEntity(const std::string& name)
    {
        Entity* entity = nullptr;
        auto itr = m_Entities.find(name);

        if (itr == m_Entities.end())
        {
            entity = new Entity(name);
            m_Entities.insert(std::make_pair(name, entity));
        }
        else
        {
            Log("Tried to create an entity with a name that already exists.", LOG_MINIMAL);
        }

        return entity;
    }

    void World::DestroyEntity(Entity* entity)
    {
        if (entity == nullptr)
        {
            Log("Tried to delete an entity with a naff pointer", LOG_MINIMAL);
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
            Log("Tried to delete an entity that does not seem to exist in the map.", LOG_MINIMAL);
        }
    }

    const bool World::IsLoaded() const
    {
        return m_IsLoaded;
    }

    void World::Update(const float& deltaTime)
    {
        for (auto& ent : m_Entities)
        {
            ent.second->Update(deltaTime);
        }
    }
    void World::Render(const Renderer& renderer)
    {
        for (auto& ent : m_Entities)
        {
            ent.second->Render(renderer);
        }
    }
}