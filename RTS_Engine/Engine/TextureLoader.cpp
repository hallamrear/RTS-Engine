#include "BennettPCH.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Buffer.h"

namespace Bennett
{
	Texture* TextureLoader::Load(Renderer& renderer, const std::string& filepath)
	{
		return Load(renderer, filepath.c_str());
	}

	Texture* TextureLoader::Load(Renderer& renderer, const char* filepath)
	{
		Texture* texture = new Texture();
		Texture::Create(*texture, renderer, filepath);
		return texture;

		return nullptr;
	}
}