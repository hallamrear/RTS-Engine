#pragma once
#include <vector>
#include <unordered_map>
#include <World/Actor/BActor.h>

namespace Bennett
{
	class BENNETT_ENGINE PhysicsWorld
	{
	private:
		std::unordered_map<std::string, BProp*> m_PhysicsObjectList;

	public:
		PhysicsWorld();
		~PhysicsWorld();

		void RegisterPhysicsEntity(BProp* object);
		void DeregisterPhysicsEntity(BProp* object);

		void Prestep(const float& deltaTime);
		void Update(const float& deltaTime);
		void Render();
	};
}

