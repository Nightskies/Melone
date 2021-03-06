#include "EditorLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Melone/Scene/SceneSerializer.h"

#include "Melone/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Melone/Math/Math.h"

namespace Melone
{
	EditorLayer::EditorLayer()
		:
		Layer("EditorLayer"),
		mEditorCamera({ 1280.0f, 720.0f })
	{
		EventSystem::Subscribe(this, &EditorLayer::OnKeyPressed);
	}

	void EditorLayer::OnAttach()
	{
		mCheckerboardTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");
		FramebufferSpecification fbSpec = { 1280, 720 };
		mFramebuffer = Framebuffer::Create(fbSpec);

		mActiveScene = Scene::Create();
#if 0
		// Entity
		auto square = mActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto redSquare = mActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		mSquareEntity = square;

		mCameraEntity = mActiveScene->CreateEntity("Camera A");
		mCameraEntity.AddComponent<CameraComponent>();

		mSecondCamera = mActiveScene->CreateEntity("Camera B");
		auto& cc = mSecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Melone::Key::A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(Melone::Key::D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(Melone::Key::W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(Melone::Key::S))
					translation.y -= speed * ts;
			}
		};

		mCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		mSecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		mSceneHierarchyPanel.SetContext(mActiveScene);

	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		// Resize
		if (FramebufferSpecification spec = mFramebuffer->GetSpecification();
			mViewportSize.x > 0.0f && mViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != mViewportSize.x || spec.Height != mViewportSize.y))
		{
			mFramebuffer->Resize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
			mEditorCamera.SetViewportDimensions({ mViewportSize.x, mViewportSize.y });

			mActiveScene->OnViewportResize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
		}

		// Update
		if (mViewportFocused)
			mEditorCamera.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		mFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Update scene
		mActiveScene->OnUpdateEditorCamera(ts, mEditorCamera);

		mFramebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool optFullscreenPersistant = true;
		bool optFullscreen = optFullscreenPersistant;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (optFullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar();

		if (optFullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("FILE"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save As..."))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) 
					Melone::Application::Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		mSceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		auto stats = Melone::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		mViewportFocused = ImGui::IsWindowFocused();
		mViewportHovered = ImGui::IsWindowHovered();
		if (!mViewportFocused && !mViewportHovered)
		{
			EventSystem::BlockEvents(EventCategoryInput);
		}
		else
		{
			EventSystem::UnBlockEvents();
		}

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		unsigned int textureID = mFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Gizmos
		Entity selectedEntity = mSceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && mGizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Runtime camera from entity
			/*auto cameraEntity = mActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

			// Editor camera
			const glm::mat4& cameraProjection = mEditorCamera.GetProjectionMatrix();
			glm::mat4 cameraView = mEditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (mGizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)mGizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnKeyPressed(const KeyPressedEvent& e)
	{
		if (e.IsKeyRepeat())
			return;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
				NewScene();

			break;
		}
		case Key::O:
		{
			if (control)
				OpenScene();

			break;
		}
		case Key::S:
		{
			if (control && shift)
				SaveSceneAs();

			break;
		}

		// Gizmos
		case Key::Q:
			mGizmoType = -1;
			break;
		case Key::W:
			MELONE_CORE_INFO("KEY");
			mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::E:
			mGizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::R:
			mGizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
	}

	void EditorLayer::NewScene()
	{
		mActiveScene = std::make_shared<Scene>();
		mActiveScene->OnViewportResize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		auto filePath = FileDialogs::OpenFile("Melone Scene (*.melone)\0*.melone\0");
		if (filePath)
		{
			mActiveScene = std::make_shared<Scene>();
			mActiveScene->OnViewportResize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
			mSceneHierarchyPanel.SetContext(mActiveScene);

			SceneSerializer serializer(mActiveScene);
			serializer.Deserialize(std::move(*filePath));
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		auto filePath = FileDialogs::SaveFile("Melone Scene (*.melone)\0*.melone\0");
		if (filePath)
		{
			SceneSerializer serializer(mActiveScene);
			serializer.Serialize(std::move(*filePath));
		}
	}
}