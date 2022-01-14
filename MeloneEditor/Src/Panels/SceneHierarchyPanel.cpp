#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include "Melone/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>

namespace Melone 
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& context)
	{
		setContext(context);
	}

	void SceneHierarchyPanel::setContext(const std::shared_ptr<Scene>& context)
	{
		mContext = context;
	}

	void SceneHierarchyPanel::onImGuiRender(void)
	{
		ImGui::Begin("Scene Hierarchy");

		mContext->mRegistry.each([&](auto entityID)
		{
			Entity entity{ entityID , mContext.get() };
			drawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			mSelectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (mSelectionContext)
			drawComponents(mSelectionContext);

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(unsigned int)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			mSelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::drawComponents(Entity entity)
	{
		if (entity.hasComponent<TagComponent>())
		{
			auto& tag = entity.getComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.hasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.getComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}
	}
}