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
	extern const std::filesystem::path AssetPath;

	EditorLayer::EditorLayer()
		:
		Layer("EditorLayer"),
		mEditorCamera({ 1280.0f, 720.0f })
	{
		EventSystem::Subscribe(this, &EditorLayer::OnKeyPressed);
		EventSystem::Subscribe(this, &EditorLayer::OnMouseButtonPressed);
	}

	void EditorLayer::OnAttach()
	{
		mCheckerboardTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");
		mIconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		mIconStop = Texture2D::Create("Resources/Icons/StopButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
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

		// Render
		Renderer2D::ResetStats();
		mFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		mFramebuffer->ClearAttachment(1, -1);

		// Update scene
		switch (mSceneState)
		{
			case SceneState::Edit:
			{	
				mEditorCamera.OnUpdate(ts);

				mActiveScene->OnUpdateEditorCamera(ts, mEditorCamera);
				break;
			}
			case SceneState::Play:
			{
				mActiveScene->OnUpdate(ts);
				break;
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= mViewportBounds[0].x;
		my -= mViewportBounds[0].y;
		glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = mFramebuffer->ReadPixel(1, mouseX, mouseY);
			mHoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, mActiveScene.get());
		}

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
		mContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		std::string name = "None";
		if (mHoveredEntity)
			name = mHoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Melone::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

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

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}


		// Gizmos
		Entity selectedEntity = mSceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && mGizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y, mViewportBounds[1].x - mViewportBounds[0].x, mViewportBounds[1].y - mViewportBounds[0].y);

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

		UIToolbar();

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

	void EditorLayer::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
	{
		if (e.GetPressedButton() == Mouse::ButtonLeft)
		{
			if (mViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				mSceneHierarchyPanel.SetSelectedEntity(mHoveredEntity);
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
			OpenScene(*filePath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		mActiveScene = std::make_shared<Scene>();
		mActiveScene->OnViewportResize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
		mSceneHierarchyPanel.SetContext(mActiveScene);

		SceneSerializer serializer(mActiveScene);
		serializer.Deserialize(path.string());
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

	void EditorLayer::UIToolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		SPtr<Texture2D> icon = mSceneState == SceneState::Edit ? mIconPlay : mIconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (mSceneState == SceneState::Edit)
				OnScenePlay();
			else if (mSceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay()
	{
		mSceneState = SceneState::Play;
		mActiveScene->OnRuntimeStart();
	}

	void EditorLayer::OnSceneStop()
	{
		mSceneState = SceneState::Edit;
		mActiveScene->OnRuntimeStop();
	}
}