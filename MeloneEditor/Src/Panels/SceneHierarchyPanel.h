#pragma once

#include "Melone/Core/Core.h"
#include "Melone/Core/Log.h"
#include "Melone/Scene/Scene.h"
#include "Melone/Scene/Entity.h"

namespace Melone 
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(void) = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		void setContext(const std::shared_ptr<Scene>& scene);

		void onImGuiRender(void);
	private:
		void drawEntityNode(Entity entity);
	private:
		std::shared_ptr<Scene> mContext;
		Entity mSelectionContext;
	};
}