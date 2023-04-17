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

	void Model::Render(const Renderer& renderer)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, Position);
		matrix = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		renderer.UniformMatrixBuffer.Model = matrix;

		if (m_Meshes.size() <= 0)
		{
			Log("Trying to draw a model with 0 meshes.", LOG_MINIMAL);
			return;
		}

		for (size_t i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Bind(renderer);
			m_Meshes[i]->Render(renderer);
		}
	}
}