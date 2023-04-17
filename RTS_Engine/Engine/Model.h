#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

namespace Bennett
{
	class Renderer;

	class Model
	{
	friend class ModelLoader;

	protected:
		std::vector<Mesh*> m_Meshes;
		Model();

	public:
		glm::vec3 Position;
		float Rotation;

		~Model();

		void Render(const Renderer& renderer);
	};
}

