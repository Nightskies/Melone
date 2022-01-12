#pragma once

#include "Scene.h"

namespace Melone 
{
	class Entity
	{
	private:
		entt::entity mEntityHandle{ entt::null };
		Scene* mScene = nullptr;
	public:
		Entity(void) = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			MELONE_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
			return mScene->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent(void)
		{
			MELONE_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			return mScene->mRegistry.get<T>(mEntityHandle);	
		}

		template<typename T>
		bool hasComponent(void)
		{
			return mScene->mRegistry.all_of<T>(mEntityHandle);
		}

		template<typename T>
		void removeComponent(void)
		{
			MELONE_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			mScene->mRegistry.remove<T>(mEntityHandle);
		}

		operator bool() const { return mEntityHandle != entt::null; }
	};

}

