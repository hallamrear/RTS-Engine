#include "BennettPCH.h"
#include "TestUnit.h"
#include <Rendering/Renderer.h>

namespace Bennett
{
	TestUnit::TestUnit(const std::string& name, const Transform& transform)
		: BActor(name, transform)
	{

	}

	TestUnit::~TestUnit()
	{

	}

	void TestUnit::Render(const Renderer& renderer)
	{
		BActor::Render(renderer);

		if (IsSelected())
		{
			renderer.DrawDebugCircle(GetTransform().GetPosition(), 1.0f);
		}

		if (GetTargetPosition() != NullTargetPosition)
		{
			renderer.DrawDebugLine(GetTransform().GetPosition(), GetTargetPosition());
		}
	}
}