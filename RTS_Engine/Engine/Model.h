#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

namespace Bennett
{
	class Model
	{
	private:
		std::vector<Mesh*> m_Meshes;

	public:
		Model(const Renderer& renderer);
		~Model();

		void Render(const Renderer& renderer);
	};
}

