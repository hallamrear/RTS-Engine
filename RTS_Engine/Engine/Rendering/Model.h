#pragma once
#include <Rendering/VertexBuffer.h>
#include <Rendering/IndexBuffer.h>
#include <Rendering/Mesh.h>

namespace Bennett
{
	class Renderer;
	class Texture;

	class BENNETT_ENGINE Model
	{
	friend class ModelLoader;

	protected:
		std::vector<Mesh*> m_Meshes;
		Model();

	public:
		~Model();

		const std::vector<Mesh*>& GetMeshes() const;

		void Render(const Renderer& renderer) const;
	};
}

