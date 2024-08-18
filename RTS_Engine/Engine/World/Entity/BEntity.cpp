#include <BennettPCH.h>
#include <World/Entity/BEntity.h>
#include <Rendering/Renderer.h>

namespace Bennett
{
	EntityID BEntity::m_EntityIDCounter = 0;

	BEntity::BEntity(const std::string& name, const Transform& transform)
	{
		m_Name = name;
		m_Transform = transform;
		m_ID = m_EntityIDCounter;
		m_EntityIDCounter++;
	}

	BEntity::~BEntity()
	{
		m_ID = -1;
	}

	void BEntity::Update(const float& deltaTime)
	{
		m_Transform.UpdateBasisVectors();
	}

	void BEntity::Render(const Renderer& renderer)
	{
		glm::mat4 model = m_Transform.GetModelMatrix();
		renderer.PushConstants.ModelMatrix = model;
	}

	const std::string& BEntity::GetName() const
	{
		return m_Name;
	}

	const EntityID& BEntity::GetID() const
	{
		return m_ID;
	}
}
