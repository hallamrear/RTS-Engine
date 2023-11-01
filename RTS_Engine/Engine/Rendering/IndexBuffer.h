#pragma once
#include <Rendering/Buffer.h>
#include <Rendering/Renderer.h>

namespace Bennett
{
	class BENNETT_ENGINE IndexBuffer : public Buffer
	{
	private:

	public:
		void Bind() override;

		static bool Create(IndexBuffer& buffer, const std::vector<unsigned int>& indices);
		static void Destroy(IndexBuffer& buffer);
	};
}