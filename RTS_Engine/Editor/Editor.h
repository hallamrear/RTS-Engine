#pragma once
#include <System/Engine.h>

namespace Bennett
{
	class Window;
}

LRESULT CALLBACK MainWindowWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

class Editor : public Bennett::Engine
{
private:
	Bennett::Window* m_MainWindow;
	Bennett::Window* m_RenderWindow;
	Bennett::Window* m_PropertiesWindow;
	Bennett::Window* m_ToolWindow;
	Bennett::Window* m_HierarchyWindow;
	bool CreateWindows();

public:
	bool Initialise();
	Editor();
	~Editor();
	
	void RunGameLoop();
};