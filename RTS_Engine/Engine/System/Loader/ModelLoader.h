#pragma once

namespace Bennett
{
	class Renderer;
	class Vertex;

	class BENNETT_ENGINE ModelLoader
	{
		friend class Model;
		friend class AssetManager;

	protected:
		static Model* Load(const std::string& filepath);
		static Model* Load(const char* filepath);
		static Model* Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	};
}
