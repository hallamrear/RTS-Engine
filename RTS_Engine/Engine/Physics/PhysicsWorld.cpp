#include "BennettPCH.h"
#include "PhysicsWorld.h"
#include "Collision/CollisionDetection.h"
#include "Collision/CollisionResolution.h"
#include "Collision/CollisionStructures.h"

namespace Bennett
{
	PhysicsWorld::PhysicsWorld()
	{
		m_PhysicsObjectList = std::unordered_map<std::string, BProp*>();
	}

	PhysicsWorld::~PhysicsWorld()
	{
		m_PhysicsObjectList.clear();
	}

	void PhysicsWorld::RegisterPhysicsEntity(BProp* object)
	{
		auto found = m_PhysicsObjectList.find(object->GetName());

		if (found != m_PhysicsObjectList.end())
		{
			Log(LOG_MINIMAL, "Trying to register entity \"%s\" in physics world when it already exists.", object->GetName().c_str());
			return;
		}

		m_PhysicsObjectList.insert(std::make_pair(object->GetName(), object));
	}

	void PhysicsWorld::DeregisterPhysicsEntity(BProp* object)
	{
		auto found = m_PhysicsObjectList.find(object->GetName());

		if (found == m_PhysicsObjectList.end())
		{
			Log(LOG_MINIMAL, "Trying to remove entity \"%s\" in physics world when it does not exist.", object->GetName().c_str());
			return;
		}

		m_PhysicsObjectList.erase(object->GetName());
	}

	void PhysicsWorld::Prestep(const float& deltaTime)
	{

	}

	void PhysicsWorld::Update(const float& deltaTime)
	{
		Collider* colliderI = nullptr;
		Collider* colliderJ = nullptr;

		for (auto i = m_PhysicsObjectList.begin(); i != m_PhysicsObjectList.end(); i++)
		{
			for (auto j = i; j != m_PhysicsObjectList.end(); j++)
			{
				if (i->second == j->second)
					continue;

				colliderI = i->second->GetPhysicsCollider();
				colliderJ = j->second->GetPhysicsCollider();

				if ((colliderI == nullptr) || (colliderJ == nullptr))
					continue;

				if ((colliderI->GetType() != ColliderType::Sphere) || (colliderJ->GetType() != ColliderType::Sphere))
					continue;

				CollisionDetails manifold;

				auto scI = (SphereCollider*)colliderI;
				auto scJ = (SphereCollider*)colliderJ;


				if (Collision::SphereSphere(*scI, *scJ, &manifold))
				{
					CollisionResolution::SoftResolveCollision(i->second->GetRigidbody(), j->second->GetRigidbody(), manifold);
				}
			}
		}
	}

	void PhysicsWorld::Render()
	{

	}
}