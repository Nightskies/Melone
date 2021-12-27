#include "mlpch.h"
#include "Renderer2D.h"

#include "ArrayObj.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Melone
{
	struct Renderer2DStorage
	{
		std::shared_ptr<VAO> QuadVertexArray;
		std::shared_ptr<Shader> TextureShader;
		std::shared_ptr<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* sData;

	void Renderer2D::init(void)
	{
		sData = new Renderer2DStorage();
		sData->QuadVertexArray = VAO::create();

		float squareVertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<VBO> squareVBO = VBO::create(squareVertices, sizeof(squareVertices));
		squareVBO->setLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float2, "aTexCoord" }
			});
		sData->QuadVertexArray->addVBO(squareVBO);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IBO> squareIBO = IBO::create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int));
		sData->QuadVertexArray->setIBO(squareIBO);

		sData->WhiteTexture = Texture2D::create(1, 1);
		unsigned int whiteTextureData = 0xffffffff;
		sData->WhiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

		sData->TextureShader = Shader::create("Assets/Shaders/TextureShader.glsl");
		sData->TextureShader->bind();
		sData->TextureShader->setUniformInt("uTexture", 0);
	}

	void Renderer2D::shutdown(void)
	{
		delete sData;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		sData->TextureShader->bind();
		sData->TextureShader->setUniformMat4("uViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene(void)
	{
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		sData->TextureShader->setUniformFloat4("uColor", color);
		sData->TextureShader->setUniformFloat("uTilingFactor", 1.0f);
		sData->WhiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData->TextureShader->setUniformMat4("uTransform", transform);
		sData->QuadVertexArray->bind();
		RenderCommand::drawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		sData->TextureShader->setUniformFloat4("uColor", tintColor);
		sData->TextureShader->setUniformFloat("uTilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		sData->TextureShader->setUniformMat4("uTransform", transform);

		sData->QuadVertexArray->bind();
		RenderCommand::drawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		sData->TextureShader->setUniformFloat4("uColor", color);
		sData->TextureShader->setUniformFloat("uTilingFactor", 1.0f);
		sData->WhiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		sData->TextureShader->setUniformMat4("uTransform", transform);
		sData->QuadVertexArray->bind();

		RenderCommand::drawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		sData->TextureShader->setUniformFloat4("uColor", tintColor);
		sData->TextureShader->setUniformFloat("uTilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		sData->TextureShader->setUniformMat4("uTransform", transform);

		sData->QuadVertexArray->bind();
		RenderCommand::drawIndexed(sData->QuadVertexArray);
	}
}