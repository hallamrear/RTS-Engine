#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <World/Entity.h>
#include <World/World.h>
#include <World/WorldChunk.h>
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

    void World::AddEntityToSpatialGrid(const Entity& entity)
    {
        glm::ivec2 id = GetChunkIDOfPosition(entity.GetTransform().GetPosition());

        auto itr = m_SpatialGrid.find(id);

        //Found id in grid.
        if (itr != m_SpatialGrid.end())
        {
            if (itr->second != nullptr)
            {
                itr->second->AddEntity(entity);
                return;
            }
            else
            {
                Log(LOG_SERIOUS, "Found grid cell %i, %i in spatial grid but world chunk did not exist.\n", (int)id.x, (int)id.y);
            }
        }
        else
        {
            Log(LOG_MINIMAL, "Failed to find ID %i, %i in spatial grid.\n", (int)id.x, (int)id.y);
            Log(LOG_MINIMAL, "Creating new chunk.\n");
            WorldChunk* chunk = WorldChunk::Create(id);
            m_SpatialGrid.insert(std::make_pair(id, chunk));
            Log(LOG_MINIMAL, "Created chunk %i, %i.\n", (int)id.x, (int)id.y);
        }

    }

    void World::RemoveEntityFromSpatialGrid(const Entity& entity)
    {
        glm::vec2 id = GetChunkIDOfPosition(entity.GetTransform().GetPosition());

        auto itr = m_SpatialGrid.find(id);

        //Found id in grid.
        if (itr != m_SpatialGrid.end())
        {
            if (itr->second != nullptr)
            {
                itr->second->RemoveEntity(entity);
            }
            else
            {
                Log(LOG_MINIMAL, "Found grid cell %i, %i in spatial grid but world chunk did not exist.\n", (int)id.x, (int)id.y);
            }
        }
        else
        {
            Log(LOG_MINIMAL, "Failed to find ID %i, %i in spatial grid for removal of %s and so has not be safely removed.\n", (int)id.x, (int)id.y, entity.GetName());
        }
    }

    glm::ivec2 World::GetChunkIDOfPosition(const glm::vec3& position)
    {
        return glm::ivec2(floorf(position.x / WorldChunkSize), floorf(position.z / WorldChunkSize));
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
            RemoveEntityFromSpatialGrid(*entity);

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

            AddEntityToSpatialGrid(*entity);

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
        for (auto& chunk : m_SpatialGrid)
        {
            chunk.second->RemoveAllEntities();
            chunk.second->Update(deltaTime);
        }
       
        for (auto& entity : m_Entities)
        {
            AddEntityToSpatialGrid(*entity.second);
            entity.second->Update(deltaTime);
        }

    }

    void World::Render(const Renderer& renderer)
    {
        for (auto& entity : m_Entities)
        {
            entity.second->Render(renderer);
        }

        for (auto& chunk : m_SpatialGrid)
        {
            chunk.second->Render(renderer);
        }
    }
}