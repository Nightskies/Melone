#pragma once

#include <Melone.h>

class Sandbox2D : public Melone::Layer
{
public:
	Sandbox2D(void);
	~Sandbox2D(void) = default;

	virtual void onAttach(void) override;
	virtual void onDetach(void) override;
	virtual void onUpdate(Melone::Timestep ts) override;
	virtual void onImGuiRender(void) override;
	virtual void onEvent(Melone::Event& e) override;
private:
	Melone::OrthographicCameraController mCameraController;

	// Temp
	std::shared_ptr<Melone::VAO> mSquareVAO;
	std::shared_ptr<Melone::Shader> mFlatColorShader;
	std::shared_ptr<Melone::Texture2D> mCheckerboardTexture;

	glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
