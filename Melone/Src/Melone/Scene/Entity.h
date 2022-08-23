#pragma once

#include "Scene.h"
#include "Melone/Core/UUID.h"
#include "Components.h"

namespace Melone 
{
	class Entity
	{
	private:
		entt::entity mEntityHandle{ entt::null };
		Scene* mScene = nullptr; //weak ptr
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			MELONE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = mScene->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
			mScene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			MELONE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return mScene->mRegistry.get<T>(mEntityHandle);	
		}

		template<typename T>
		bool HasComponent()
		{
			return mScene->mRegistry.all_of<T>(mEntityHandle); 
		}

		template<typename T>
		void RemoveComponent()
		{
			MELONE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			mScene->mRegistry.remove<T>(mEntityHandle);
		}

		operator bool() const { return mEntityHandle != entt::null; }
		operator entt::entity() const { return mEntityHandle; }
		operator unsigned int() const { return (unsigned int)mEntityHandle; }

		bool operator==(const Entity& other) const
		{
			return mEntityHandle == other.mEntityHandle && mScene == other.mScene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
	};

}

