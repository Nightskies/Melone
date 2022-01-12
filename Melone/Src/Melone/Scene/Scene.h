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
		unsigned int mViewportWidth = 0;
		unsigned int mViewportHeight = 0;
	public:
		Scene(void);
		~Scene(void);

		Entity createEntity(const std::string& name);

		void onViewportResize(unsigned int width, unsigned int height);
		void onUpdate(Timestep ts);
	};
}

