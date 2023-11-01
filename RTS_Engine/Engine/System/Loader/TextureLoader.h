#pragma once

namespace Bennett
{
	class Renderer;

	class BENNETT_ENGINE TextureLoader
	{
	private:
		friend class Texture;
		friend class AssetManager;
		static int m_TextureIDCounter;

	protected:
		static Texture* Load(const std::string& filepath);
		static Texture* Load(const char* filepath);
	};
}

