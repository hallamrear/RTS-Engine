#pragma once

namespace tinygltf
{
	class Model;
}

namespace Bennett
{
	class Renderer;
	class Vertex;
	class Model;

	class BENNETT_ENGINE ModelLoader
	{
		friend class Model;
		friend class AssetManager;

	private:
		static bool LoadMeshData(Model& model, const tinygltf::Model& gltf);

	protected:
		static Model* Load(const std::string& filepath, const bool& isBinary);
		static Model* Load(const char* filepath, const bool& isBinary);

		static Model* Create(const std::vector<Vertex>& vertices, const std::vector<VertexIndex>& indices);
	};
}
