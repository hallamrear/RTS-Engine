#pragma once

namespace Bennett
{
	class Renderer;

	class TextureLoader
	{
		friend class Texture;
		friend class AssetManager;

	protected:
		static Texture* Load(Renderer& renderer, const std::string& filepath);
		static Texture* Load(Renderer& renderer, const char* filepath);
	};
}

