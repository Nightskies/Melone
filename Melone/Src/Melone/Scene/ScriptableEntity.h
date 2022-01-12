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
		template<typename T>
		T& getComponent(void)
		{
			return mEntity.getComponent<T>();
		}
	};
}