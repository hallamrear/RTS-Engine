#include "BennettPCH.h"
#include "Model.h"

namespace Bennett
{
	Model::Model()
	{
		m_Meshes = std::vector<Mesh*>();
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

	void Model::Render(const Renderer& renderer) const
	{
		if (m_Meshes.size() <= 0)
		{
			Log("Trying to draw a model with 0 meshes.", LOG_MINIMAL);
			return;
		}

		renderer.BindDescriptorSet();
		renderer.UpdateUniformBuffers();

		for (size_t i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Bind(renderer);
			m_Meshes[i]->Render(renderer);
		}
	}
}