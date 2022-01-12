#include "mlpch.h"
#include "Entity.h"

namespace Melone
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: 
		mEntityHandle(handle), 
		mScene(scene)
	{}
}