#include <BennettPCH.h>
#include <Rendering/Renderer.h>
#include <World/Entity/BEntity.h>
#include <World/Actor/UnitActors/TestUnit.h>
#include <World/Prop/Objects/StaticProp.h>
#include <World/World.h>
#include <World/WorldChunk.h>
#include <World/Terrain/TerrainChunk.h>
#include <Physics/Collision/CollisionResolution.h>

namespace Bennett
{
    void World::DeleteAllEntities()
    {
        for (auto itr = m_Entities.begin(); itr != m_Entities.end(); /*it++*/)
        {
            BEntity* entity = itr->second;
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

    void World::AddEntityToSpatialGrid(BEntity& entity)
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
            CreateChunk(id)->AddEntity(entity);
        }
    }

    WorldChunk* World::CreateChunk(const glm::ivec2& id)
    {
        auto itr = m_SpatialGrid.find(id);

        //Found id in grid.
        if (itr == m_SpatialGrid.end())
        {
            Log(LOG_MINIMAL, "Failed to find ID %i, %i in spatial grid.\n", (int)id.x, (int)id.y);
            Log(LOG_MINIMAL, "Creating new chunk.\n");
            WorldChunk* chunk = WorldChunk::Create(id);
            m_SpatialGrid.insert(std::make_pair(id, chunk));
            Log(LOG_MINIMAL, "Created chunk %i, %i.\n", (int)id.x, (int)id.y);
            return chunk;
        }

        Log(LOG_MINIMAL, "Tried to create a world chunk with ID %i, %i in spatial grid but one already exists.\n", (int)id.x, (int)id.y);

        return itr->second;
    }

    void World::RemoveEntityFromSpatialGrid(BEntity& entity)
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

    glm::ivec2 World::GetChunkIDOfPosition(const glm::vec3& position) const
    {
        return glm::ivec2(floorf(position.x / WorldChunkSize), floorf(position.z / WorldChunkSize));
    }

    WorldChunk* World::GetWorldChunk(const glm::vec3& position) const
    {
        return GetWorldChunk(GetChunkIDOfPosition(position));
    }

    WorldChunk* World::GetWorldChunk(const glm::ivec2& id) const
    {
        auto itr = m_SpatialGrid.find(id);

        //Found id in grid.
        if (itr != m_SpatialGrid.end())
        {
            if (itr->second != nullptr)
            {
                return itr->second;
            }
            else
            {
                Log(LOG_SERIOUS, "Found grid cell %i, %i in spatial grid but world chunk did not exist.\n", (int)id.x, (int)id.y);
            }
        }
        else
        {
            Log(LOG_MINIMAL, "Failed to find ID %i, %i in spatial grid.\n", (int)id.x, (int)id.y);
        }

        return nullptr;
    }

    World::World()
    {
        m_Entities = std::unordered_map<std::string, BEntity*>();
        m_IsLoaded = false;
    }

    World::~World()
    {
        for (auto itr = m_Entities.begin(); itr != m_Entities.end(); /*it++*/)
        {
            BEntity* entity = itr->second;
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

    void World::DestroyEntity(BEntity* entity)
    {
        if (entity == nullptr)
        {
            Log(LOG_MINIMAL, "World: Tried to delete an entity with an invalid pointer\n");
            return;
        }

        auto itr = m_Entities.find(entity->GetName());
        if (itr != m_Entities.end())
        {
            BProp* prop = dynamic_cast<BProp*>(entity);
            if (prop)
            {
                m_PhysicsWorld.DeregisterPhysicsEntity(prop);
            }

            RemoveEntityFromSpatialGrid(*entity);

            delete itr->second;
            itr->second = nullptr;
            m_Entities.erase(itr);
        }
        else
        {
            Log(LOG_MINIMAL, "World: Tried to delete an entity that does not seem to exist in the map.\n");
        }
    }

    void World::PreloadChunks(const std::vector<glm::ivec2>& chunkIds)
    {
        for (size_t i = 0; i < chunkIds.size(); i++)
        {
            CreateChunk(chunkIds[i]);
        }
    }

    float World::GetTerrainHeight(const glm::vec3& position) const
    {
        glm::ivec2 id = GetChunkIDOfPosition(position);
        WorldChunk* chunk = GetWorldChunk(id);

        if (chunk != nullptr)
        {
            if (chunk->m_TerrainChunk)
            {
                int scaling = (WorldChunkSize / ChunkCellWidth);

                glm::vec3 relativePosition = position - chunk->m_TerrainChunk->GetChunkCornerPosition();
                glm::ivec2 relativeCellLocation{};
                relativeCellLocation.x = ((int)relativePosition.x % WorldChunkSize) / scaling;
                relativeCellLocation.y = ((int)relativePosition.z % WorldChunkSize) / scaling;
                return chunk->m_TerrainChunk->GetHeightAtPointInCell(relativeCellLocation);
            }
        }

        return 0.0f;
    }

    const bool World::IsLoaded() const
    {
        return m_IsLoaded;
    }

    BActor* World::SpawnActor(const std::string& name, const Transform& transform)
    {
        BActor* entity = nullptr;
        auto itr = m_Entities.find(name);

        if (itr == m_Entities.end())
        {
            entity = new TestUnit(name, transform);
            m_Entities.insert(std::make_pair(name, entity));
            m_Actors.insert(std::make_pair(name, entity));
            m_PhysicsWorld.RegisterPhysicsEntity(entity);

            AddEntityToSpatialGrid(*entity);

            if (ENABLE_LOG_SPAWN_ENTITY_NOTICE)
            {
                Log(LOG_SAFE, "World: Created an actor called \"%s\"\n", name.c_str());
            }
        }
        else
        {
            Log(LOG_MINIMAL, "World: Tried to create an actor with a name that already exists.\n");
        }

        return entity;
    }

    void World::GetAllActors(std::vector<BActor*>& actors)
    {
        actors.clear();
        actors.reserve(m_Actors.size());
        for (auto& itr : m_Actors)
        {
            actors.push_back(itr.second);
        }
    }

    BActor* World::GetActor(const std::string& name)
    {
        BActor* entity = nullptr;
        auto itr = m_Actors.find(name);

        if (itr != m_Actors.end())
        {
            return itr->second;
        }

        return entity;
    }

    BProp* World::SpawnProp(const std::string& name, const Transform& transform)
    {
        BProp* entity = nullptr;
        auto itr = m_Entities.find(name);

        if (itr == m_Entities.end())
        {
            entity = new StaticProp(name, transform);
            m_Entities.insert(std::make_pair(name, entity));
            m_Props.insert(std::make_pair(name, entity));
            m_PhysicsWorld.RegisterPhysicsEntity(entity);

            AddEntityToSpatialGrid(*entity);

            if (ENABLE_LOG_SPAWN_ENTITY_NOTICE)
            {
                Log(LOG_SAFE, "World: Created a prop called \"%s\"\n", name.c_str());
            }
        }
        else
        {
            Log(LOG_MINIMAL, "World: Tried to create a prop with a name that already exists.\n");
        }

        return entity;
    }

    BProp* World::GetProp(const std::string& name)
    {
        BProp* entity = nullptr;
        auto itr = m_Props.find(name);

        if (itr != m_Props.end())
        {
            return itr->second;
        }

        return entity;
    }

    void World::Update(const float& deltaTime)
    {
        m_PhysicsWorld.Prestep(deltaTime);
        m_PhysicsWorld.Update(deltaTime);

        for (auto& chunk : m_SpatialGrid)
        {
            chunk.second->RemoveAllEntities();
        }

        for (auto& entity : m_Entities)
        {
            AddEntityToSpatialGrid(*entity.second);
            entity.second->Update(deltaTime);
        }

        for (auto& chunk : m_SpatialGrid)
        {
            chunk.second->Update(deltaTime);
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