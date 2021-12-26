#include "ExampleLayer.h"

#include <glm/gtc/matrix_transform.hpp>

ExampleLayer::ExampleLayer(void)
	:
	Layer("Example"),
	mCameraController(1280.0f / 720.0f, true)
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

	mCameraController.onEvent(e);
}

void ExampleLayer::onUpdate(Melone::Timestep ts)
{
	mCameraController.onUpdate(ts);

	Melone::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Melone::RenderCommand::clear();

	Melone::Renderer::beginScene(mCameraController.getCamera());

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