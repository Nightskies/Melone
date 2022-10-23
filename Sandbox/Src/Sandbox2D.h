#pragma once

//#include <Melone.h>

#include "ParticleSystem.h"

class Sandbox2D : public Melone::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() override = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Melone::Timestep ts) override;
	void OnImGuiRender() override;
private:
	Melone::Camera mCamera;

	// Temp
	SPtr<Melone::VAO> mSquareVAO;
	SPtr<Melone::Shader> mFlatColorShader;
	SPtr<Melone::Texture2D> mCheckerboardTexture;
	SPtr<Melone::Texture2D> mSomePicture;
	SPtr<Melone::Texture2D> mSpriteSheet;

	SPtr<Melone::SubTexture2D> mTextureStairs;
	SPtr<Melone::SubTexture2D> mTextureTree;
	SPtr<Melone::SubTexture2D> mTextureWindow;

	glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleProps mParticle;
	ParticleSystem mParticleSystem;

	unsigned int mMapWidth;
	unsigned int mMapHeight;
	std::unordered_map<char, SPtr<Melone::SubTexture2D>> sTextureMap;
};
