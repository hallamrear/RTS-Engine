#include "BennettPCH.h"
#include "TestUnit.h"
#include <Rendering/Renderer.h>
#include <System/Assets/AssetManager.h>
#include <Rendering/Model.h>
#include <Physics/Collision/Collider/Collider.h>

namespace Bennett
{
	TestUnit::TestUnit(const std::string& name, const Transform& transform)
		: BActor(name, transform)
	{
		SetModel(AssetManager::GetModel("TestUnit.gltf"));
		SetTexture(AssetManager::GetTexture("TestUnit"));

		GetSelectionCollider().SetRadius(10.0f);

		if (HasModel())
		{
			float maxExtentsLength = GetModel()->GetMeshes()[0]->GetMaxExtents().length();
			float minExtentsLength = GetModel()->GetMeshes()[0]->GetMinExtents().length();
			GetSelectionCollider().SetRadius(glm::max(maxExtentsLength, minExtentsLength));
		}

		GeneratePhysicsColliderFromModel(ColliderType::Sphere);
	}

	TestUnit::~TestUnit()
	{
		DestroyPhysicsCollider();
	}

	void TestUnit::Render(const Renderer& renderer)
	{
		BActor::Render(renderer);

		if (IsSelected())
		{
			renderer.DrawDebugCircle(GetTransform().GetPosition(), GetSelectionCollider().GetRadius());
		}

		if (GetTargetPosition() != NullTargetPosition)
		{
			renderer.DrawDebugLine(GetTransform().GetPosition(), GetTargetPosition());
		}
	}
}