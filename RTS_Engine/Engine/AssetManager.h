#pragma once
#include "ModelLoader.h"
#include "TextureLoader.h"

namespace Bennett
{
	class Model;
	class Texture;
	class Renderer;

	class AssetManager
	{
	private:
		static std::unordered_map<std::string, Model*> m_ModelMap;
		static std::unordered_map<std::string, Texture*> m_TextureMap;

	public:
		static Model* GetModel(const Renderer& renderer, const std::string& modelName);
		static Texture* GetTexture(const Renderer& renderer, const std::string& textureName);
	};
}

