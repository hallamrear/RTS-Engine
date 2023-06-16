#pragma once

namespace Bennett
{
	class Renderer;

	class ModelLoader
	{
		friend class Model;
		friend class AssetManager;

	protected:
		static Model* Load(const std::string& filepath);
		static Model* Load(const char* filepath);
	};
}
