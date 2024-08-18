#pragma once
#include <Defines/BennettDefines.h>
#include <System/Transform.h>
#include <string>

namespace Bennett
{
	typedef unsigned int EntityID;

	enum class ColliderType;
	class Renderer;

	class BENNETT_ENGINE BEntity
	{
	private:
		static EntityID m_EntityIDCounter;
		EntityID m_ID;
		std::string m_Name;
		Transform m_Transform;

	protected:
		BEntity(const std::string& name,
			const Transform& transform);

	public:
		virtual ~BEntity() = 0;

		virtual void Update(const float& deltaTime) = 0;
		virtual void Render(const Renderer& renderer) = 0;

		const std::string& GetName() const;
		const EntityID& GetID() const;

		inline Transform& GetTransform() { return m_Transform; };
		inline const Transform& GetTransform() const { return m_Transform; };
	};
}