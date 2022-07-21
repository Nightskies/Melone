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

Sandbox2D::Sandbox2D()
	:
	Layer("Sandbox2D"),
	mCamera()
{}

void Sandbox2D::OnAttach()
{
	mCheckerboardTexture = Melone::Texture2D::Create("Assets/Textures/Checkerboard.png");

#if 0
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

#endif

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Melone::Timestep ts)
{
#if 0
	// Update
	mCamera.OnUpdate(ts);

	// Render
	Melone::Renderer2D::ResetStats();
	Melone::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Melone::RenderCommand::Clear();

	static float rotation = 0.0f;
	rotation += ts * 50.0f;

	Melone::Renderer2D::BeginScene(mCamera);

	Melone::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Melone::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Melone::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, mSquareColor);
	Melone::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, mCheckerboardTexture, 10.0f);
	Melone::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, mCheckerboardTexture, 20.0f);
		
	Melone::Renderer2D::EndScene();

	Melone::Renderer2D::BeginScene(mCamera);
	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
			Melone::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}
	Melone::Renderer2D::EndScene();


	if (Melone::Input::IsMouseButtonPressed(Melone::Mouse::ButtonLeft));
	{
		auto [x, y] = Melone::Input::GetMousePosition();
		auto [w, h] = mProperties->Dimensions;

		// bounds width and bounds height
		auto [bw, bh] = mCameraController.GetBounds().GetCameraDimentions();
		auto pos = mCameraController.GetCamera().GetPos();

		x = (x / w) * bw - bw * 0.5f;
		y = bh * 0.5f - (y / h) * bh;
		mParticle.Position = { x + pos.x, y + pos.y };

		for (int i = 0; i < 5; i++)
			mParticleSystem.Emit(mParticle);
	}

	mParticleSystem.OnUpdate(ts);
	mParticleSystem.OnRender(mCameraController.GetCamera());


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

void Sandbox2D::OnImGuiRender()
{
		ImGui::Begin("Settings");

		auto stats = Melone::Renderer2D::GetStats();

		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	    ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
}