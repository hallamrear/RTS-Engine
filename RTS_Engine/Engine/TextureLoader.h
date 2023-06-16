#pragma once

namespace Bennett
{
	class Renderer;

	class TextureLoader
	{
		friend class Texture;
		friend class AssetManager;

	protected:
		static Texture* Load(const std::string& filepath);
		static Texture* Load(const char* filepath);
	};
}

