#include "BennettPCH.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Buffer.h"

namespace Bennett
{
	Texture* TextureLoader::Load(const std::string& filepath)
	{
		return Load(filepath.c_str());
	}

	Texture* TextureLoader::Load(const char* filepath)
	{
		Texture* texture = new Texture();
		Texture::Create(*texture, filepath);
		return texture;
	}
}