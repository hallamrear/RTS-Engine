#pragma once
#include "Camera.h"
#include <vector>

namespace Bennett
{
	class InputMonitor;

	enum CAMERA_MODE
	{
		FREE_CAM = 0,
		STANDARD_CAM = 1,
		SCRIPTED_CAMERA = 2,
		_COUNT = 3
	};

	class CameraController
	{
	private:
		friend Camera;

		static CameraController* m_Instance;

		CAMERA_MODE m_CurrentMode;

		Camera** m_Cameras;

		void CreateCameras();

	protected:
		CameraController();

	public:
		~CameraController();

		static CameraController& Get();
		void SetCamera(CAMERA_MODE mode);
		Camera& GetCurrentCamera() const;
		Camera& GetCamera(CAMERA_MODE mode) const;
	};
}