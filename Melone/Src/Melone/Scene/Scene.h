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

		entt::registry mRegistry;
	public:
		Scene(void);
		~Scene(void);

		Entity Scene::createEntity(const std::string& name);

		// temp
		entt::registry& reg(void) { return mRegistry; }

		void onUpdate(Timestep ts);
	};
}

