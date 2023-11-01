#pragma once
#include <System/Loader/ModelLoader.h>
#include <System/Loader/TextureLoader.h>

namespace Bennett
{
	class Model;
	class Texture;
	class ServiceLocator;

	class BENNETT_ENGINE AssetManager
	{
		friend ServiceLocator;

	private:
		static std::unordered_map<std::string, Model*> m_ModelMap;
		static std::unordered_map<std::string, Texture*> m_TextureMap;

	protected:
		AssetManager();

	public:
		~AssetManager();

		static Model* CreateModel(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		static Model* GetModel(const std::string& modelName);
		static Texture* GetTexture(const std::string& textureName);
	};
}

