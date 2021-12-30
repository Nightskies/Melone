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

#if 0
	mSpriteSheet = Melone::Texture2D::create("Assets/Game/Textures/RPGpack_sheet_2X.png");

	mTextureStairs = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 7, 6 }, { 128, 128 });
	mTextureWindow = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 10, 2 }, { 128, 128 });
	mTextureTree = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	sTextureMap['D'] = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 6, 11 }, { 128, 128 });
	sTextureMap['W'] = Melone::SubTexture2D::createFromCoords(mSpriteSheet, { 11, 11 }, { 128, 128 });

	mMapWidth = sMapWidth;
	mMapHeight = strlen(sMapTiles) / sMapWidth;
#endif

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
		ImGui::Begin("Settings");

		auto stats = Melone::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

		ImGui::End();
}

void Sandbox2D::onEvent(Melone::Event& e)
{
	mCameraController.onEvent(e);
}
