#include <BennettPCH.h>
#include <System/Assets/TextureLoader.h>
#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>
#include <Rendering/Buffer.h>

namespace Bennett
{
	int TextureLoader::m_TextureIDCounter = -1;

	Texture* TextureLoader::Load(const std::string& filepath)
	{
		return Load(filepath.c_str());
	}

	Texture* TextureLoader::Load(const char* filepath)
	{
		Texture* texture = new Texture();
		Texture::Create(*texture, filepath);
		m_TextureIDCounter++;
		return texture;
	}
}