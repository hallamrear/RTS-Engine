#include <BennettPCH.h>
#include <Defines/BennettDefines.h>
#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>
#include <Rendering/Model.h>
#include <System/Assets/ModelLoader.h>
#include <System/Assets/TextureLoader.h>
#include <System/Assets/AssetManager.h>
#include <System/ServiceLocator.h>

namespace Bennett
{
    std::unordered_map<std::string, Model*> AssetManager::m_ModelMap = std::unordered_map<std::string, Model*>();
    std::unordered_map<std::string, Texture*> AssetManager::m_TextureMap = std::unordered_map<std::string, Texture*>();

    AssetManager::AssetManager()
    {

    }

    AssetManager::~AssetManager()
    {
        std::unordered_map<std::string, Model*>::iterator modelItr = m_ModelMap.begin();
        while (modelItr != m_ModelMap.end())
        {
            if (modelItr->second)
            {
                delete modelItr->second;
                modelItr->second = nullptr;
            }
            modelItr++;
        }
        m_ModelMap.clear();

        std::unordered_map<std::string, Texture*>::iterator textureItr = m_TextureMap.begin();
        while (textureItr != m_TextureMap.end())
        {
            if (textureItr->second)
            {
                Texture::Destroy(*textureItr->second);
                delete textureItr->second;
                textureItr->second = nullptr;
            }
            textureItr++;
        }
        m_TextureMap.clear();
    }

    Model* AssetManager::CreateModel(const std::string& modelName, const std::vector<Vertex>& vertices, const std::vector<VertexIndex>& indices)
    {
        auto found = m_ModelMap.find(modelName);

        if (found != m_ModelMap.end())
        {
            Log(LOG_MINIMAL, "Attempted to create a model from scratch using a name of a model already loaded. Returning existing model.");
            return found->second;
        }

        Model* model = ModelLoader::Create(vertices, indices);
        if (!model)
        {
            Log(LOG_SERIOUS, "Failed to create model from scratch.");
            return nullptr;
        }

        m_ModelMap.insert(std::make_pair(modelName, model));

        return model;
    }

    Model* AssetManager::GetModel(const std::string& modelName)
    {
        auto found = m_ModelMap.find(modelName);

        if (found != m_ModelMap.end())
        {
            return found->second;
        }

        std::string filepath = ServiceLocator::GetResourceFolderLocation() + modelName;

        Model* model = ModelLoader::Load(filepath.c_str(), false);
        if (!model)
        {
            Log(LOG_SERIOUS, "Failed to load model");
            return nullptr;
        }

        m_ModelMap.insert(std::make_pair(modelName, model));
        
        return model;
    }

    Texture* AssetManager::GetTexture(const std::string& textureName)
    {
        auto found = m_TextureMap.find(textureName);

        if (found != m_TextureMap.end())
        {
            return found->second;
        }

        std::string filepath = ServiceLocator::GetResourceFolderLocation() + textureName + ".png";

        Texture* texture = TextureLoader::Load(filepath.c_str());
        if (!texture)
        {
            Log(LOG_SERIOUS, "Failed to load texture %s", filepath.c_str());
            return nullptr;
        }

        m_TextureMap.insert(std::make_pair(textureName, texture));

        std::vector<Texture*> textures;
        int loadedCount = 0;
        for (auto& tex : m_TextureMap)
        {
            if (loadedCount > MAX_LOADED_TEXTURES)
            {
                break;
            }
            
            textures.push_back(tex.second);
        }

        return texture;
    }
}
