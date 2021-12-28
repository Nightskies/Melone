#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(void)
	:
	Layer("Sandbox2D"),
	mCameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::onAttach(void)
{
	mCheckerboardTexture = Melone::Texture2D::create("Assets/Textures/Checkerboard.png");
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

	Melone::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Melone::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Melone::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Melone::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, mCheckerboardTexture, 10.0f);
	Melone::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, mCheckerboardTexture, 20.0f);

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
