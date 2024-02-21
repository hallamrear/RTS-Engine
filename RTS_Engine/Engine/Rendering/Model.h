#pragma once
#include <Rendering/VertexBuffer.h>
#include <Rendering/IndexBuffer.h>
#include <Rendering/Mesh.h>

namespace Bennett
{
	class Renderer;
	class Texture;

	class BENNETT_ENGINE Model
	{
	friend class ModelLoader;
		const Texture* m_Texture;

	protected:
		std::vector<Mesh*> m_Meshes;
		Model();

	public:
		~Model();

		const std::vector<Mesh*>& GetMeshes() const;

		inline const Texture& GetTexture()		   const { return *m_Texture; };
		void SetTexture(const Texture* texture);
		bool HasTexture() const { return m_Texture != nullptr; };

		void Render(const Renderer& renderer) const;
	};
}

