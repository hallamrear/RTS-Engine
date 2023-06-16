#pragma once
#include "AssetManager.h"
#include "Renderer.h"
#include "Window.h"

namespace Bennett
{
	class Window;
	class Renderer;
	class AssetManager;

	class ServiceLocator
	{
	private:
		static Renderer* m_Renderer;
		static AssetManager* m_AssetManager;
		static Window* m_Window;

	public:
		static bool Initialise();
		static void Shutdown();

		inline static AssetManager& GetAssetManager() { assert(m_AssetManager != nullptr); return *m_AssetManager; };
		inline static Renderer& GetRenderer() { assert(m_Renderer != nullptr); return *m_Renderer; };
		inline static Window& GetWindow() { assert(m_Window != nullptr); return *m_Window; };
	};
}