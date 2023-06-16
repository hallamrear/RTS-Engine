#pragma once
#include "ModelLoader.h"
#include "TextureLoader.h"

namespace Bennett
{
	class Model;
	class Texture;
	class ServiceLocator;

	class AssetManager
	{
		friend ServiceLocator;

	private:
		static std::unordered_map<std::string, Model*> m_ModelMap;
		static std::unordered_map<std::string, Texture*> m_TextureMap;

	protected:
		AssetManager();

	public:
		~AssetManager();

		static Model* GetModel(const std::string& modelName);
		static Texture* GetTexture(const std::string& textureName);
	};
}

