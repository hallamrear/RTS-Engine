#pragma once

namespace Bennett
{
	class Window;
	class Renderer;
	class AssetManager;

	class BENNETT_ENGINE ServiceLocator
	{
	private:
		static Renderer* m_Renderer;
		static AssetManager* m_AssetManager;
		static Window* m_Window;
		static std::string m_ResourceFolderLocation;

		static bool LoadSettings();

	public:
		static bool Initialise(Window& renderWindow);
		static void Shutdown();

		inline static AssetManager& GetAssetManager() { assert(m_AssetManager != nullptr); return *m_AssetManager; };
		inline static Renderer& GetRenderer() { assert(m_Renderer != nullptr); return *m_Renderer; };
		inline static Window& GetWindow() { assert(m_Window != nullptr); return *m_Window; };

		inline static const std::string& GetResourceFolderLocation() { return m_ResourceFolderLocation; };
	};
}