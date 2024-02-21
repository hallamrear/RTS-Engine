#pragma once
#include <Defines/BennettDefines.h>
#include <Rendering/Model.h>
#include <System/Transform.h>
#include <string>

namespace Bennett
{
	enum class ColliderType;
	class Model;
	class Renderer;
	class Collider;

	class BENNETT_ENGINE Entity
	{
	private:
		std::string m_Name;
		Transform m_Transform;
		Model* m_Model;
		Collider* m_Collider;

	protected:
		class InputMonitor* m_InputMonitor;

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

		Collider* GetCollider() const { if (m_Collider) return m_Collider; else return nullptr; };
		bool HasCollider()  const { return m_Model != nullptr; };

		const std::string& GetName() const;

		inline Transform& GetTransform() { return m_Transform; };

		void GenerateBroadPhaseColliderFromModel(ColliderType type);
		void AddBroadPhaseCollider(ColliderType type, glm::vec3 size);
		void AddNarrowPhaseCollider(ColliderType type, glm::vec3 size);
	};
}