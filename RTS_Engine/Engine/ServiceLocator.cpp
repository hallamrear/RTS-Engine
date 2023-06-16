#include "BennettPCH.h"
#include "ServiceLocator.h"

namespace Bennett
{
    Renderer* ServiceLocator::m_Renderer = nullptr;
    AssetManager* ServiceLocator::m_AssetManager = nullptr;
    Window* ServiceLocator::m_Window = nullptr;

    bool ServiceLocator::Initialise()
    {
        m_Window = new Window();
        m_Renderer = new Renderer();
        m_Renderer->Initialise(*m_Window);
        m_AssetManager = new AssetManager();

        return (m_Window != nullptr) && (m_Renderer != nullptr) && (m_AssetManager != nullptr);
    }

    void ServiceLocator::Shutdown()
    {
        if (m_Renderer)
        {
            m_Renderer->Shutdown();
            delete m_Renderer;
            m_Renderer = nullptr;
        }

        if (m_AssetManager)
        {
            delete m_AssetManager;
            m_AssetManager = nullptr;
        }

    }

}