#pragma once
#include <Defines/BennettDefines.h>
#include <Rendering/Model.h>
#include <System/Transform.h>
#include <string>

namespace Bennett
{
	typedef unsigned int EntityID;

	enum class ColliderType;
	class Model;
	class Renderer;
	class Collider;

	class BENNETT_ENGINE Entity
	{
	private:
		static EntityID m_EntityIDCounter;
		EntityID m_ID;
		std::string m_Name;
		Transform m_Transform;
		Model* m_Model;
		const Texture* m_Texture;
		Collider* m_Collider;

	protected:
		class InputMonitor* m_InputMonitor;

		inline const Texture& GetTexture()		   const { return *m_Texture; };

		void DestroyCollider();


	public:
		Entity(const std::string& name,
			const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f));

		Entity(const std::string& name,
			const Transform& transform);

		virtual ~Entity();

		virtual void Update(const float& deltaTime);
		virtual void Render(const Renderer& renderer);

		inline Model* GetModel() { return m_Model; };
		void SetModel(Model* model);
		bool HasModel()   const { return m_Model != nullptr; };

		void SetTexture(Texture* texture);
		bool HasTexture()   const { return m_Texture != nullptr; };

		Collider* GetCollider() const { if (m_Collider) return m_Collider; else return nullptr; };
		bool HasCollider()  const { return m_Collider != nullptr; };

		const std::string& GetName() const;
		const EntityID& GetID() const;

		inline Transform& GetTransform() { return m_Transform; };
		inline const Transform& GetTransform() const { return m_Transform; };

		void GenerateBroadPhaseColliderFromModel(const ColliderType& type);
		void AddBroadPhaseCollider(const ColliderType& type, const glm::vec3& size, const glm::vec3& offset = glm::vec3(0.0f));
		void AddNarrowPhaseCollider(const ColliderType& type, const glm::vec3& size, const glm::vec3& offset = glm::vec3(0.0f));
	};
}