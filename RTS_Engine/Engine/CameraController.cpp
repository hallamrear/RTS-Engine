#include "BennettPCH.h"
#include "CameraController.h"
#include "InputMonitor.h"
#include "FreeCamera.h"
#include "StandardCamera.h"
#include "ScriptedCamera.h"
#include "Logger.h"

namespace Bennett
{
    CameraController* CameraController::m_Instance = nullptr;

    void CameraController::CreateCameras()
    {
        m_Cameras = new Camera * [CAMERA_MODE::_COUNT];
        m_Cameras[0] = new FreeCamera();
        m_Cameras[1] = new StandardCamera();
        m_Cameras[2] = new ScriptedCamera();
    }

    CameraController::CameraController()
    {
        m_CurrentMode = FREE_CAM;
    }

    CameraController::~CameraController()
    {
        for (size_t i = 0; i < CAMERA_MODE::_COUNT; i++)
        {
            delete m_Cameras[i];
            m_Cameras[i] = nullptr;
        }

        delete[] m_Cameras;
        m_Cameras = nullptr;
    }

    CameraController& CameraController::Get()
    {
        if (!m_Instance)
        {
            m_Instance = new CameraController();
            m_Instance->CreateCameras();
        }

        return *m_Instance;
    }

    void CameraController::SetCamera(CAMERA_MODE mode)
    {
        if (m_CurrentMode == mode)
            return;

        std::string str = "Changed to camera: ";
        switch (mode)
        {
        case Bennett::FREE_CAM: str += "Free Camera"; break;
        case Bennett::STANDARD_CAM: str += "Standard Camera"; break;
        case Bennett::SCRIPTED_CAMERA: str += "Scripted Camera"; break;
        default:
            return;
            break;
        }

        Log(str, LOG_SAFE);
        m_CurrentMode = mode;
    }

    Camera& CameraController::GetCurrentCamera() const
    {
        return *m_Cameras[m_CurrentMode];
    }

    Camera& CameraController::GetCamera(CAMERA_MODE mode) const
    {
        return *m_Cameras[mode];
    }
}