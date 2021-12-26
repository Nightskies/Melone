#pragma once

#include <Melone.h>

class ExampleLayer: public Melone::Layer
{
private:
	std::shared_ptr<Melone::Shader> mSquareShader;
	std::shared_ptr<Melone::Shader> mSquareTextureShader;
	std::shared_ptr<Melone::Texture2D> mTexture;
	std::shared_ptr<Melone::VAO> mVAO;
	std::shared_ptr<Melone::VBO> mVBO;
	std::shared_ptr<Melone::IBO> mIBO;

	Melone::OrthographicCamera mCamera;

	glm::vec3 mCameraPosition = { 0.0f, 0.0f, 0.0f };

	float mCameraRotation = 0.0f;
	float mCameraTranslationSpeed = 5.0f;
	float mCameraRotationSpeed = 180.0f;

	glm::vec3 mSquareColor = { 1.0f, 0.5f, 0.3f };
public:
	ExampleLayer(void);

	void onEvent(Melone::Event& e) override;

	void onUpdate(Melone::Timestep ts) override;
};