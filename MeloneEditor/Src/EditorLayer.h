#pragma once

#include <Melone.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Melone/Renderer/Camera/EditorCamera.h"

namespace Melone 
{
	class EditorLayer : public Layer
	{
	private:
		EditorCamera mEditorCamera;

		SPtr<VAO> mSquareVAO;
		SPtr<Shader> mFlatColorShader;
		SPtr<Framebuffer> mFramebuffer;

		SPtr<Texture2D> mCheckerboardTexture;

		SPtr<Scene> mActiveScene;
		SPtr<Scene> mEditorScene;
		std::filesystem::path mEditorScenePath;

		Entity mSquareEntity;
		Entity mCameraEntity;
		Entity mSecondCamera;
		Entity mHoveredEntity;

		bool mPrimaryCamera = true;

		glm::vec2 mViewportSize = { 0.0f, 0.0f };

		glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		bool mViewportFocused = false;
		bool mViewportHovered = false;

		int mGizmoType = -1;

		glm::vec2 mViewportBounds[2];

		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState mSceneState = SceneState::Edit;

		bool mShowPhysicsColliders = false;

		// Editor resources
		SPtr<Texture2D> mIconPlay;
		SPtr<Texture2D> mIconStop;

		// Panels
		SceneHierarchyPanel mSceneHierarchyPanel;
		ContentBrowserPanel mContentBrowserPanel;
	public:
		EditorLayer();
		~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
	private:
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(SPtr<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();
		void OnDuplicateEntity();

		void OnOverlayRender();

		void UIToolbar();

		void OnKeyPressed(const KeyPressedEvent& e);
		void OnMouseButtonPressed(const MouseButtonPressedEvent& e);
	};
}