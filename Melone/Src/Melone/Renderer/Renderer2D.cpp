#include "mlpch.h"
#include "Renderer2D.h"

#include "ArrayObj.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Melone
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DStash
	{
		const unsigned int MaxQuads = 10000;
		const unsigned int MaxVertices = MaxQuads * 4;
		const unsigned int MaxIndices = MaxQuads * 6;

		std::shared_ptr<VAO> QuadVAO;
		std::shared_ptr<VBO> QuadVBO;
		std::shared_ptr<Shader> TextureShader;
		std::shared_ptr<Texture2D> WhiteTexture;

		unsigned int QuadIndexCount = 0;
		QuadVertex* QuadVBOBase = nullptr;
		QuadVertex* QuadVBOPtr = nullptr;
	};

	static Renderer2DStash sData;

	void Renderer2D::init(void)
	{
		sData.QuadVAO = VAO::create();

		sData.QuadVBO = VBO::create(sData.MaxVertices * sizeof(QuadVertex));
		sData.QuadVBO->setLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float2, "aTexCoord" }
			});
		sData.QuadVAO->addVBO(sData.QuadVBO);

		sData.QuadVBOBase = new QuadVertex[sData.MaxVertices];

		unsigned int* quadIndices = new unsigned int[sData.MaxIndices];

		unsigned int offset = 0;
		for (unsigned int i = 0; i < sData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IBO> squareIBO = IBO::create(quadIndices, sData.MaxIndices);
		sData.QuadVAO->setIBO(squareIBO);
		delete[] quadIndices;

		sData.WhiteTexture = Texture2D::create(1, 1);
		unsigned int whiteTextureData = 0xffffffff;
		sData.WhiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

		sData.TextureShader = Shader::create("Assets/Shaders/TextureShader.glsl");
		sData.TextureShader->bind();
		sData.TextureShader->setUniformInt("uTexture", 0);
	}

	void Renderer2D::shutdown(void)
	{

	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		sData.TextureShader->bind();
		sData.TextureShader->setUniformMat4("uViewProjection", camera.getViewProjectionMatrix());

		sData.QuadIndexCount = 0;
		sData.QuadVBOPtr = sData.QuadVBOBase;
	}

	void Renderer2D::endScene(void)
	{
		unsigned int dataSize = (uint8_t*)sData.QuadVBOPtr - (uint8_t*)sData.QuadVBOBase;
		sData.QuadVBO->setData(sData.QuadVBOBase, dataSize);

		flush();
	}

	void Renderer2D::flush(void)
	{
		RenderCommand::drawIndexed(sData.QuadVAO, sData.QuadIndexCount);
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
		sData.QuadVBOPtr->Position = position;
		sData.QuadVBOPtr->Color = color;
		sData.QuadVBOPtr->TexCoord = { 0.0f, 0.0f };
		sData.QuadVBOPtr++;

		sData.QuadVBOPtr->Position = { position.x + size.x, position.y, 0.0f };
		sData.QuadVBOPtr->Color = color;
		sData.QuadVBOPtr->TexCoord = { 1.0f, 0.0f };
		sData.QuadVBOPtr++;

		sData.QuadVBOPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		sData.QuadVBOPtr->Color = color;
		sData.QuadVBOPtr->TexCoord = { 1.0f, 1.0f };
		sData.QuadVBOPtr++;

		sData.QuadVBOPtr->Position = { position.x, position.y + size.y, 0.0f };
		sData.QuadVBOPtr->Color = color;
		sData.QuadVBOPtr->TexCoord = { 0.0f, 1.0f };
		sData.QuadVBOPtr++;

		sData.QuadIndexCount += 6;

		/*sData.TextureShader->setUniformFloat4("uColor", color);
		sData.TextureShader->setUniformFloat("uTilingFactor", 1.0f);
		sData.WhiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.TextureShader->setUniformMat4("uTransform", transform);
		sData.QuadVAO->bind();
		RenderCommand::drawIndexed(sData.QuadVAO);*/
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		sData.TextureShader->setUniformFloat4("uColor", tintColor);
		sData.TextureShader->setUniformFloat("uTilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		sData.TextureShader->setUniformMat4("uTransform", transform);

		sData.QuadVAO->bind();
		RenderCommand::drawIndexed(sData.QuadVAO);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		sData.TextureShader->setUniformFloat4("uColor", color);
		sData.TextureShader->setUniformFloat("uTilingFactor", 1.0f);
		sData.WhiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		sData.TextureShader->setUniformMat4("uTransform", transform);
		sData.QuadVAO->bind();

		RenderCommand::drawIndexed(sData.QuadVAO);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		sData.TextureShader->setUniformFloat4("uColor", tintColor);
		sData.TextureShader->setUniformFloat("uTilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		sData.TextureShader->setUniformMat4("uTransform", transform);

		sData.QuadVAO->bind();
		RenderCommand::drawIndexed(sData.QuadVAO);
	}
}