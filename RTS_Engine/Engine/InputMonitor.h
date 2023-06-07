#pragma once
#include <vector>
#include <map>

class GLFWwindow;

namespace Bennett
{
	class InputMonitor
	{
	private:
		static GLFWwindow* m_AttachedWindow;
		static bool m_IsAttached;
		static std::vector<InputMonitor*> m_Instances;
		static void GLFWInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		bool m_IsEnabled;
		std::map<int, bool> m_TrackedKeys;

	protected:
		void SetKeyState(int key, bool state);

	public:
		InputMonitor(const std::vector<int>& keyIdsToTrack);
		~InputMonitor();

		glm::vec2 GetMousePosition() const;
		void SetMousePositionToCentre() const;
		bool GetKeyState(int key);
		bool GetIsEnabled();
		void SetIsEnabled(const bool& state);

		/// <summary>
		/// Only needs to be done once using the window.
		/// </summary>
		/// <param name="window"></param>
		static void AttachToWindow(GLFWwindow& window);
	};
}