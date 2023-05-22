#pragma once

namespace Bennett
{
	class AssetManager;
	class Renderer;

	class Services
	{
	private:
		enum class SERVICE_NAME
		{
			RENDERER,
			ASSET_MANAGER
		};

		static AssetManager m_AssetManager;
		static Renderer m_Renderer;

	public:
		static void Initialise();
		static void Shutdown();

		static AssetManager& GetAsset() { return m_AssetManager; };
		static Renderer& Get() { return m_Renderer; };
	};
}

