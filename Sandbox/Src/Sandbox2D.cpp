#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(void)
	:
	Layer("Sandbox2D"),
	mCameraController(1280.0f / 720.0f)
{}

void Sandbox2D::onAttach(void)
{
	mCheckerboardTexture = Melone::Texture2D::create("Assets/Textures/Checkerboard.png");
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Melone::Timestep ts)
{
	// Update
	mCameraController.onUpdate(ts);

	// Render
	Melone::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Melone::RenderCommand::clear();

	Melone::Renderer2D::beginScene(mCameraController.getCamera());

	Melone::Renderer2D::drawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
	Melone::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Melone::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, mCheckerboardTexture);

	Melone::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender(void)
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Melone::Event& e)
{
	mCameraController.onEvent(e);
}
