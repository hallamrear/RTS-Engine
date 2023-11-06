#pragma once
#include <Defines/BennettInputDefines.h>
#include <vector>
#include <map>

namespace Bennett
{
	class Engine;
	class Window;

	class BENNETT_ENGINE InputMonitor
	{
	private:
		static Window* m_AttachedWindow;
		static bool m_IsAttached;
		static std::vector<InputMonitor*> m_Instances;

		bool m_IsEnabled;
		bool m_IsCursorLocked;
		std::map<int, bool> m_TrackedKeys;

		static void KeyboardInputCallback(int vkKey, bool state, bool repeat);
		static void MouseMovementInputCallback(const glm::vec2& mousePosition);

	protected:
		void SetKeyState(int key, bool state);

	public:
		InputMonitor(const std::vector<int>& keyIdsToTrack);
		~InputMonitor();

		void LockCursor();
		void UnlockCursor();
		const bool& IsCursorLocked() const;
		void ToggleCursorLock();

		glm::vec2 GetMousePosition() const;
		void SetMousePositionToCentre() const;
		bool GetKeyState(int key);
		bool GetIsEnabled() const;
		void SetIsEnabled(const bool& state);

		/// <summary>
		/// Only needs to be done once using the window.
		/// </summary>
		static void AttachToWindow(Window& window);
		static void Win32InputCallback(WIN32MSG msg, LPARAM lParam, WPARAM wParam);
	};
}