#pragma once
#include <World/Entity/BEntity.h>
#include <Physics/Rigidbody.h>

namespace Bennett
{
	class Model;
	class Texture;
	class Collider;

	class BENNETT_ENGINE BProp
		: public BEntity
	{
	private:
		Model* m_Model;
		const Texture* m_Texture;
		Collider* m_PhysicsCollider;
		Rigidbody m_Rigidbody;

	protected:
		BProp(const std::string& name, const Transform& transform);

	public:
		virtual ~BProp();

		Rigidbody& GetRigidbody();

		Model* GetModel();
		void SetModel(Model* model);
		bool HasModel() const;

		const Texture& GetTexture() const;
		void SetTexture(Texture* texture);
		bool HasTexture() const;

		Collider* GetPhysicsCollider() const;
		bool HasPhysicsCollider() const;
		void DestroyPhysicsCollider();

		void GeneratePhysicsColliderFromModel(const ColliderType& type);
		void AddCollider(const ColliderType& type, const glm::vec3& size, const glm::vec3& offset = glm::vec3(0.0f));

		virtual void Update(const float& deltaTime) override;
		virtual void Render(const Renderer& renderer) override;

	};
}