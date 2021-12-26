#include "ExampleLayer.h"

#include <glm/gtc/matrix_transform.hpp>

ExampleLayer::ExampleLayer(void)
	:
	Layer("Example"),
	mCamera(-1.6f, 1.6f, -0.9f, 0.9f)
{
	/* Square */
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		0, 2, 3
	};

	mTexture = Melone::Texture2D::create("Assets/Textures/FirstTexture.png");

	mVBO.reset(Melone::VBO::create(vertices, sizeof(vertices)));
	mVBO->setLayout({
		{ Melone::ShaderDataType::Float3, "aPosition" },
		{ Melone::ShaderDataType::Float2, "aTexCoord" }
		});

	mIBO.reset(Melone::IBO::create(indices, sizeof(indices) / sizeof(unsigned int)));

	mVAO = Melone::VAO::create();
	mVAO->addVBO(mVBO);
	mVAO->setIBO(mIBO);

	auto textureShader = mShaderLibrary.load("Assets/Shaders/TextureShader.glsl");

	mSquareShader = Melone::Shader::create("Assets/Shaders/FlatColor.glsl");

	std::dynamic_pointer_cast<Melone::OpenGLShader>(textureShader)->bind();
	std::dynamic_pointer_cast<Melone::OpenGLShader>(textureShader)->setUniformInt("uTexture", 0);
}

void ExampleLayer::onEvent(Melone::Event& e)
{
	MELONE_INFO("{0}", e.toString());

	if (e.getEventType() == Melone::EventType::KeyPressed)
	{
		Melone::KeyPressedEvent& ev = static_cast<Melone::KeyPressedEvent&>(e);

		if (ev.getKeyCode() == MELONE_KEY_TAB)
			MELONE_TRACE("Tab key is pressed (event)!");

		MELONE_TRACE("{0}", (char)ev.getKeyCode());
	}
}

void ExampleLayer::onUpdate(Melone::Timestep ts)
{
	if (Melone::Input::isKeyPressed(MELONE_KEY_A))
		mCameraPosition.x -= mCameraTranslationSpeed * ts;
	else if (Melone::Input::isKeyPressed(MELONE_KEY_D))
		mCameraPosition.x += mCameraTranslationSpeed * ts;

	if (Melone::Input::isKeyPressed(MELONE_KEY_W))
		mCameraPosition.y += mCameraTranslationSpeed * ts;
	else if (Melone::Input::isKeyPressed(MELONE_KEY_S))
		mCameraPosition.y -= mCameraTranslationSpeed * ts;

	if (Melone::Input::isKeyPressed(MELONE_KEY_Q))
		mCameraRotation += mCameraRotationSpeed * ts;
	else if (Melone::Input::isKeyPressed(MELONE_KEY_E))
		mCameraRotation -= mCameraRotationSpeed * ts;

	mCamera.setRotation(mCameraRotation);
	mCamera.setPosition(mCameraPosition);

	Melone::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Melone::RenderCommand::clear();

	Melone::Renderer::beginScene(mCamera);

	mSquareShader->bind();
	mSquareShader->setUniformFloat3("uColor", mSquareColor);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	// tiles
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Melone::Renderer::submit(mSquareShader, mVAO, transform);
		}
	}

	auto textureShader = mShaderLibrary.get("TextureShader");
	mTexture->bind();

	Melone::Renderer::submit(textureShader, mVAO);

	Melone::Renderer::endScene();
}