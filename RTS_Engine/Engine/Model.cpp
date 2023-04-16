#include "BennettPCH.h"
#include "Model.h"

namespace Bennett
{
	Model::Model(const Renderer& renderer)
	{
		m_Meshes = std::vector<Mesh*>();

		std::vector<Vertex> verts; 
		verts.push_back(Vertex(-0.5f,-0.5f, 0.0f));
		verts.push_back(Vertex( 0.5f,-0.5f, 0.0f));
		verts.push_back(Vertex( 0.5f, 0.5f, 0.0f));
		verts.push_back(Vertex(-0.5f, 0.5f, 0.0f));

		std::vector<unsigned int> inds;
		inds.push_back(0); inds.push_back(1); inds.push_back(2);
		inds.push_back(2); inds.push_back(3); inds.push_back(0);

		m_Meshes.push_back(new Mesh(renderer, verts, inds));
	}

	Model::~Model()
	{
		for (size_t i = 0; i < m_Meshes.size(); i++)
		{
			delete m_Meshes[i];
			m_Meshes[i] = nullptr;
		}

		m_Meshes.clear();
	}

	void Model::Render(const Renderer& renderer)
	{
		for (size_t i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Bind(renderer);
			m_Meshes[i]->Render(renderer);
		}
	}
}