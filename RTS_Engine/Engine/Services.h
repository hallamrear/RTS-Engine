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

		static AssetManager& Get() { return m_AssetManager; };
	};
}

