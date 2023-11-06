#include <BennettPCH.h>
#include <Camera/CameraController.h>
#include <Camera/FreeCamera.h>
#include <Camera/ScriptedCamera.h>
#include <Camera/StandardCamera.h>
#include <System/InputMonitor.h>
#include <System/Logger.h>

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

    void CameraController::DestroyCameras()
    {
        if (m_Cameras)
        {
            for (size_t i = 0; i < CAMERA_MODE::_COUNT; i++)
            {
                delete m_Cameras[i];
                m_Cameras[i] = nullptr;
            }

            delete[] m_Cameras;
            m_Cameras = nullptr;
        }
    }

    CameraController::CameraController()
    {
        m_Cameras = nullptr;
        m_CurrentMode = FREE_CAM;
    }

    CameraController::~CameraController()
    {
        DestroyCameras();
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