#pragma once

#include "Melone/Core/Core.h"
#include "Melone/Core/Log.h"
#include "Melone/Scene/Scene.h"
#include "Melone/Scene/Entity.h"

namespace Melone 
{
	class SceneHierarchyPanel
	{
	private:
		SPtr<Scene> mContext;
		Entity mSelectionContext;
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const SPtr<Scene>& scene);

		void SetContext(const SPtr<Scene>& scene);

		void OnImGuiRender();

		void SetSelectedEntity(Entity entity);
		Entity GetSelectedEntity() const { return mSelectionContext; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	};
}