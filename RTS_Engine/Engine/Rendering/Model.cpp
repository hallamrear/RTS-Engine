#include <BennettPCH.h>
#include <Rendering/Model.h>
#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>

namespace Bennett
{
	Model::Model()
	{
		m_Meshes = std::vector<Mesh*>();
		m_Texture = nullptr;
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

		//renderer.BindDescriptorSet();
		renderer.PushDescriptorSet(&GetTexture());
		renderer.UpdateUniformBuffers();

		for (size_t i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Render(renderer);
		}
	}

	void Model::SetTexture(const Texture* texture)
	{
		m_Texture = texture;
	}
}