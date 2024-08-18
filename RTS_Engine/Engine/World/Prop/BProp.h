#pragma once
#include <World/Entity/BEntity.h>

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
		Collider* m_Collider;

	protected:
		BProp(const std::string& name, const Transform& transform);

	public:
		virtual ~BProp();

		Model* GetModel();
		void SetModel(Model* model);
		bool HasModel() const;

		const Texture& GetTexture() const;
		void SetTexture(Texture* texture);
		bool HasTexture() const;

		Collider* GetCollider() const;
		bool HasCollider() const;
		void DestroyCollider();

		void GenerateColliderFromModel(const ColliderType& type);
		void AddCollider(const ColliderType& type, const glm::vec3& size, const glm::vec3& offset = glm::vec3(0.0f));

		virtual void Update(const float& deltaTime) override;
		virtual void Render(const Renderer& renderer) override;
	};
}