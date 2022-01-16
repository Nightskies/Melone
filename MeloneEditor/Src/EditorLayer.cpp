#include "EditorLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Melone
{
	EditorLayer::EditorLayer(void)
		:
		Layer("EditorLayer"),
		mCameraController(1280.0f / 720.0f, true)
	{}

	void EditorLayer::onAttach(void)
	{
		mCheckerboardTexture = Texture2D::create("Assets/Textures/Checkerboard.png");
		FramebufferSpecification fbSpec = { 1280, 720 };
		mFramebuffer = Framebuffer::create(fbSpec);

		mActiveScene = std::make_shared<Scene>();

		// Entity
		auto square = mActiveScene->createEntity("Green Square");
		square.addComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto redSquare = mActiveScene->createEntity("Red Square");
		redSquare.addComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		mSquareEntity = square;

		mCameraEntity = mActiveScene->createEntity("Camera A");
		mCameraEntity.addComponent<CameraComponent>();

		mSecondCamera = mActiveScene->createEntity("Camera B");
		auto& cc = mSecondCamera.addComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate(void)
			{
				auto& translation = getComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy(void)
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = getComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::isKeyPressed(MELONE_KEY_A))
					translation.x -= speed * ts;
				if (Input::isKeyPressed(MELONE_KEY_D))
					translation.x += speed * ts;
				if (Input::isKeyPressed(MELONE_KEY_W))
					translation.y += speed * ts;
				if (Input::isKeyPressed(MELONE_KEY_S))
					translation.y -= speed * ts;
			}
		};

		mCameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();

		mSecondCamera.addComponent<NativeScriptComponent>().bind<CameraController>();

		mSceneHierarchyPanel.setContext(mActiveScene);
	}

	void EditorLayer::onDetach(void)
	{
	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		// Resize
		if (FramebufferSpecification spec = mFramebuffer->getSpecification();
			mViewportSize.x > 0.0f && mViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != mViewportSize.x || spec.Height != mViewportSize.y))
		{
			mFramebuffer->resize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
			mCameraController.onResize(mViewportSize.x, mViewportSize.y);

			mActiveScene->onViewportResize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
		}

		// Update
		if (mViewportFocused)
			mCameraController.onUpdate(ts);

		// Render
		Renderer2D::resetStats();
		mFramebuffer->bind();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		// Update scene
		mActiveScene->onUpdate(ts);

		mFramebuffer->unbind();
	}

	void EditorLayer::onImGuiRender(void)
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

				if (ImGui::MenuItem("Exit")) Melone::App::getInstance().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		mSceneHierarchyPanel.onImGuiRender();

		ImGui::Begin("Stats");

		auto stats = Melone::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		mViewportFocused = ImGui::IsWindowFocused();
		mViewportHovered = ImGui::IsWindowHovered();
		App::getInstance().getImGuiLayer()->blockEvents(!mViewportFocused || !mViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		unsigned int textureID = mFramebuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		mCameraController.onEvent(e);
	}
}