#pragma once
#include <Physics/Rigidbody.h>
#include <Physics/Collision/CollisionStructures.h>

namespace Bennett
{
	class BENNETT_ENGINE CollisionResolution
	{
	public:
		inline static void SoftResolveCollision(Rigidbody& objectA, Rigidbody& objectB, const CollisionDetails& manifold)
		{
			if (objectA.IsStatic() && objectB.IsStatic())
				return;

            glm::vec3 vecAB = objectB.GetTransform().GetPosition() - objectA.GetTransform().GetPosition();
            vecAB = glm::normalize(vecAB);

			//Add force in the normal direction (scaled by the amount of intersection / MAX_FORCE)
            constexpr float MAX_FORCE = 50.0f;
            float force = manifold.Depth * MAX_FORCE;

            if (objectA.IsStatic() == true && objectB.IsStatic() == true)
               return;

            if (objectA.IsStatic() == false && objectB.IsStatic() == true)
            {
                objectA.GetTransform().Translate(manifold.Normal * (manifold.Depth * -1.0f));
                objectA.AddImpulseForce(manifold.Normal * (manifold.Depth * -1.0f));
            }
            else if (objectA.IsStatic() == true && objectB.IsStatic() == false)
            {
                objectB.GetTransform().Translate(manifold.Normal * (manifold.Depth * +1.0f));
                objectB.AddImpulseForce(manifold.Normal * (manifold.Depth * +1.0f));
            }
            else
            {
                objectA.AddImpulseForce(manifold.Normal * (manifold.Depth * +0.5f));
                objectA.GetTransform().Translate(manifold.Normal * (manifold.Depth * +0.5f));
                objectB.AddImpulseForce(manifold.Normal * (manifold.Depth * -0.5f));
                objectB.GetTransform().Translate(manifold.Normal * (manifold.Depth * -0.5f));
            }
		}
	};
}

