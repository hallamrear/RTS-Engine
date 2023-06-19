#include "BennettPCH.h"
#include "ServiceLocator.h"
#include "AssetManager.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"

namespace Bennett
{
    std::unordered_map<std::string, Model*> AssetManager::m_ModelMap = std::unordered_map<std::string, Model*>();
    std::unordered_map<std::string, Texture*> AssetManager::m_TextureMap = std::unordered_map<std::string, Texture*>();

    AssetManager::AssetManager()
    {

    }

    AssetManager::~AssetManager()
    {

    }

    Model* AssetManager::GetModel(const std::string& modelName)
    {
        auto found = m_ModelMap.find(modelName);

        if (found != m_ModelMap.end())
        {
            return found->second;
        }

        std::string filepath = "Assets//" + modelName + ".obj";

        Model* model = ModelLoader::Load(filepath.c_str());
        if (!model)
        {
            Log("Failed to load model", LOG_SERIOUS);
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

        std::string filepath = "Assets//" + textureName + ".png";

        Texture* texture = TextureLoader::Load(filepath.c_str());
        if (!texture)
        {
            Log("Failed to load ", LOG_SERIOUS);
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

        ServiceLocator::GetRenderer().UpdateDescriptorSets(textures);

        return texture;
    }
}
