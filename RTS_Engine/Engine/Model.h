#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

namespace Bennett
{
	class Renderer;

	class BENNETT_ENGINE Model
	{
	friend class ModelLoader;

	protected:
		std::vector<Mesh*> m_Meshes;
		Model();

	public:
		~Model();

		void Render(const Renderer& renderer) const;
	};
}

