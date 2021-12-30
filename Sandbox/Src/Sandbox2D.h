#pragma once

//#include <Melone.h>

#include "ParticleSystem.h"

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
	std::shared_ptr<Melone::Texture2D> mSpriteSheet;

	std::shared_ptr<Melone::SubTexture2D> mTextureStairs;
	std::shared_ptr<Melone::SubTexture2D> mTextureTree;
	std::shared_ptr<Melone::SubTexture2D> mTextureWindow;

	std::shared_ptr<Melone::Framebuffer> mFramebuffer;

	glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleProps mParticle;
	ParticleSystem mParticleSystem;

	unsigned int mMapWidth;
	unsigned int mMapHeight;
	std::unordered_map<char, std::shared_ptr<Melone::SubTexture2D>> sTextureMap;
};
