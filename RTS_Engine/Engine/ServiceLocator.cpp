#include "BennettPCH.h"
#include "ServiceLocator.h"

namespace Bennett
{
    Renderer* ServiceLocator::m_Renderer = nullptr;
    AssetManager* ServiceLocator::m_AssetManager = nullptr;

    void ServiceLocator::Initialise()
    {
        m_Renderer = new Renderer();
        m_AssetManager = new AssetManager();
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
            delete m_Renderer;
            m_Renderer = nullptr;
        }
    }

}