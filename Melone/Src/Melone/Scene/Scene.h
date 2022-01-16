#pragma once

#include "entt.hpp"

#include "Melone/Core/Timestep.h"

namespace Melone 
{
	class Entity;

	class Scene
	{
	private:
		friend class Entity;
		friend class SceneHierarchyPanel;

		entt::registry mRegistry;
		unsigned int mViewportWidth = 0;
		unsigned int mViewportHeight = 0;
	public:
		Scene(void);
		~Scene(void);

		Entity createEntity(const std::string& name);
		void destroyEntity(Entity entity);

		void onViewportResize(unsigned int width, unsigned int height);
		void onUpdate(Timestep ts);
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);
	};
}

