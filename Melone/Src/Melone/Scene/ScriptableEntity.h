#pragma once

#include "Entity.h"

namespace Melone
{
	class ScriptableEntity
	{
	private:
		Entity mEntity;

		friend class Scene;
	public:
		virtual ~ScriptableEntity(void) = default;

		template<typename T>
		T& getComponent(void)
		{
			return mEntity.getComponent<T>();
		}
	protected:
		virtual void OnCreate(void) {}
		virtual void OnDestroy(void) {}
		virtual void OnUpdate(Timestep ts) {}
	};
}