#pragma once
#include "Entity.h"

namespace Melone
{
	class ScriptableEntity
	{
	public:
		friend class Scene;
	public:
		ScriptableEntity() = default;
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
	private:
		Entity mEntity;
	};
}