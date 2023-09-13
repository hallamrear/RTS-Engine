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

		static AssetManager& GetAssetManager();
		static Renderer& GetRenderer();
		static Window& GetWindow();
		static const std::string& GetResourceFolderLocation();
	};	
}