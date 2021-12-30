#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const unsigned int sMapWidth = 22;
static const char* sMapTiles =
"WWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWDDDDDDDDDDWWWWWW"
"WWWWDDDDDDDDDDDDDWWWWW"
"WWWDDDDDDDDDDDDDDDWWWW"
"WWWDDDDDDDDDDDDDDDWWWW"
"WWWDDDWWWWDDDDDSDDWWWW"
"WWWDDDWWWWDDDDDDDDWWWW"
"WWWDDDDDDDDDDDDDDDWWWW"
"WWWDDDDDDDDDDDDDDWWWWW"
"WWWWWDDDDDDDDDDDWWWWWW"
"WWWWWWDDDDDDDDDWWWWWWW"
"WWWWWWWWDDDDDWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D(void)
	:
	Layer("Sandbox2D"),
	mCameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::onAttach(void)
{
	mCheckerboardTexture = Melone::Texture2D::create("Assets/Textures/Checkerboard.png");
	mSpriteSheet = Melone::Texture2D::create("Assets/Game/Textures/RPGpack_sheet_2X.png");

	mTextureStairs = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 7, 6 }, { 128, 128 });
	mTextureWindow = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 10, 2 }, { 128, 128 });
	mTextureTree = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	sTextureMap['D'] = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 6, 11 }, { 128, 128 });
	sTextureMap['W'] = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 11, 11 }, { 128, 128 });

	mMapWidth = sMapWidth;
	mMapHeight = strlen(sMapTiles) / sMapWidth;

	mParticle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	mParticle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	mParticle.SizeBegin = 0.5f, mParticle.SizeVariation = 0.3f, mParticle.SizeEnd = 0.0f;
	mParticle.LifeTime = 5.0f;
	mParticle.Velocity = { 0.0f, 0.0f };
	mParticle.VelocityVariation = { 3.0f, 1.0f };
	mParticle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::onDetach(void)
{
}

void Sandbox2D::onUpdate(Melone::Timestep ts)
{
	// Update
	mCameraController.onUpdate(ts);

	// Render
	Melone::Renderer2D::resetStats();
	Melone::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Melone::RenderCommand::clear();

	static float rotation = 0.0f;
	rotation += ts * 50.0f;

	Melone::Renderer2D::beginScene(mCameraController.getCamera());

	Melone::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
	Melone::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Melone::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Melone::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, mCheckerboardTexture, 10.0f);
	Melone::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), mCheckerboardTexture, 20.0f);

	Melone::Renderer2D::endScene();

	Melone::Renderer2D::beginScene(mCameraController.getCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
			Melone::Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}
	Melone::Renderer2D::endScene();

	if (Melone::Input::isMouseButtonPressed(MELONE_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Melone::Input::getMousePosition();
		auto [w, h] = Melone::App::getInstance().getWindow().getWinDimentions();

		// bounds width and bounds height
		auto [bw, bh] = mCameraController.getBounds().getCameraDimentions();
		auto pos = mCameraController.getCamera().getPos();

		x = (x / w) * bw - bw * 0.5f;
		y = bh * 0.5f - (y / h) * bh;
		mParticle.Position = { x + pos.x, y + pos.y };

		for (int i = 0; i < 5; i++)
			mParticleSystem.emit(mParticle);
	}

	mParticleSystem.onUpdate(ts);
	mParticleSystem.onRender(mCameraController.getCamera());

#if 0
	// Rendering sprites
	Melone::Renderer2D::beginScene(mCameraController.getCamera());

	for (size_t y = 0; y < mMapHeight; y++)
	{
		for (size_t x = 0; x < mMapWidth; x++)
		{
			char tileType = sMapTiles[x + y * mMapWidth];
			std::shared_ptr<Melone::SubTexture2D> texture;

			if (sTextureMap.find(tileType) != sTextureMap.end())
				texture = sTextureMap[tileType];
			else
				texture = mTextureWindow;

			Melone::Renderer2D::drawQuad({ x - mMapWidth / 2.0f, mMapHeight - y - mMapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
		}
	}

	//Melone::Renderer2D::drawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, mTextureStairs);
	//Melone::Renderer2D::drawQuad({ 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, mTextureWindow);
	//Melone::Renderer2D::drawQuad({ -1.0f, 0.0f, 0.5f }, { 1.0f, 2.0f }, mTextureTree);
	Melone::Renderer2D::endScene();
#endif
}

void Sandbox2D::onImGuiRender(void)
{
	static bool dockingEnabled = true;

	if (dockingEnabled)
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
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Melone::App::getInstance().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Melone::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

		uint32_t textureID = mCheckerboardTexture->getRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();

		ImGui::End();
	}
}

void Sandbox2D::onEvent(Melone::Event& e)
{
	mCameraController.onEvent(e);
}
