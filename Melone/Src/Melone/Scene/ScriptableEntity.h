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
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return mEntity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	};
}