#include "BennettPCH.h"
#include <mini/ini.h>
#include "ServiceLocator.h"
#include "AssetManager.h"
#include "Renderer.h"
#include "Window.h"

namespace Bennett
{
    std::string   ServiceLocator::m_ResourceFolderLocation = "NO RESOURCE FOLDER SET";
    AssetManager* ServiceLocator::m_AssetManager = nullptr;
    Renderer*     ServiceLocator::m_Renderer = nullptr;
    Window*       ServiceLocator::m_Window = nullptr;

    bool ServiceLocator::Initialise(Window& renderWindow)
    {
        bool loadedSettingsResult = LoadSettings();

        m_Renderer = new Renderer();
        m_Renderer->Initialise(renderWindow);
        m_Window = &renderWindow;
        m_AssetManager = new AssetManager();      

        return (m_Window != nullptr) && (m_Renderer != nullptr) && (m_AssetManager != nullptr) && loadedSettingsResult;
    }

    bool ServiceLocator::LoadSettings()
    {
        mINI::INIFile file("config.ini");
        mINI::INIStructure iniStructure;

        bool configOk = file.read(iniStructure);

        if (!configOk)
        {
            Log("Failed to read config.ini", LOG_CRITICAL);
            return false;
        }

        configOk = false;

        if (iniStructure.has("Settings"))
        {
            auto& settings = iniStructure["Settings"];
            if (settings.has("ResourceSrcFolder"))
            {
                m_ResourceFolderLocation = settings["ResourceSrcFolder"];
                Log("Resource Folder set to \"" + m_ResourceFolderLocation + "\"", LOG_SAFE);
                configOk = true;
            }
        }

        return configOk;
    }

    void ServiceLocator::Shutdown()
    {
        if (m_AssetManager)
        {
            delete m_AssetManager;
            m_AssetManager = nullptr;
        }

        if (m_Renderer)
        {
            m_Renderer->Shutdown();
            delete m_Renderer;
            m_Renderer = nullptr;
        }

        m_Window = nullptr;

        m_ResourceFolderLocation = "NO RESOURCE FOLDER SET";
    }

    AssetManager& ServiceLocator::GetAssetManager() 
    { 
        assert(m_AssetManager != nullptr);
        return *m_AssetManager; 
    };

    Renderer& ServiceLocator::GetRenderer() 
    { 
        assert(m_Renderer != nullptr);
        return *m_Renderer; 
    };

    Window& ServiceLocator::GetWindow()
    { 
        assert(m_Window != nullptr);
        return *m_Window;
    };

    const std::string& ServiceLocator::GetResourceFolderLocation()
    { 
        return m_ResourceFolderLocation;
    };
}