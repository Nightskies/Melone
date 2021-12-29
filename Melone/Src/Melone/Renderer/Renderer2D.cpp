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
		
		float TextureIndex;
		float TilingFactor;
	};

	struct Renderer2DStash
	{
		static const unsigned int MaxQuads = 10000;
		static const unsigned int MaxVertices = MaxQuads * 4;
		static const unsigned int MaxIndices = MaxQuads * 6;
		static const unsigned int MaxTextureSlots = 32;

		std::shared_ptr<VAO> QuadVAO;
		std::shared_ptr<VBO> QuadVBO;
		std::shared_ptr<Shader> TextureShader;
		std::shared_ptr<Texture2D> WhiteTexture;

		unsigned int QuadIndexCount = 0;
		QuadVertex* QuadVBOBase = nullptr;
		QuadVertex* QuadVBOPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		unsigned int TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Stats Info;
	};

	static Renderer2DStash sData;

	void Renderer2D::init(void)
	{
		sData.QuadVAO = VAO::create();

		sData.QuadVBO = VBO::create(sData.MaxVertices * sizeof(QuadVertex));
		sData.QuadVBO->setLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float2, "aTexCoord" },
			{ ShaderDataType::Float, "aTexIndex"},
			{ ShaderDataType::Float, "aTilingFactor" }
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

		int samplers[sData.MaxTextureSlots];
		for (unsigned int i = 0; i < sData.MaxTextureSlots; i++)
			samplers[i] = i;

		sData.TextureShader = Shader::create("Assets/Shaders/TextureShader.glsl");
		sData.TextureShader->bind();
		sData.TextureShader->setUniformIntArray("uTextures", samplers, sData.MaxTextureSlots);

		sData.TextureSlots[0] = sData.WhiteTexture;

		sData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		sData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		sData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		sData.TextureShader->bind();
		sData.TextureShader->setUniformMat4("uViewProjection", camera.getViewProjectionMatrix());

		sData.QuadIndexCount = 0;
		sData.QuadVBOPtr = sData.QuadVBOBase;

		sData.TextureSlotIndex = 1;
	}

	void Renderer2D::endScene(void)
	{
		unsigned int dataSize = (uint8_t*)sData.QuadVBOPtr - (uint8_t*)sData.QuadVBOBase;
		sData.QuadVBO->setData(sData.QuadVBOBase, dataSize);

		flush();
	}

	void Renderer2D::flush(void)
	{
		for (unsigned int i = 0; i < sData.TextureSlotIndex; i++)
			sData.TextureSlots[i]->bind(i);

		RenderCommand::drawIndexed(sData.QuadVAO, sData.QuadIndexCount);
		sData.Info.DrawCalls++;
	}

	void Renderer2D::flushAndReset(void)
	{
		endScene();

		sData.QuadIndexCount = 0;
		sData.QuadVBOPtr = sData.QuadVBOBase;

		sData.TextureSlotIndex = 1;
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
		constexpr float textureIndex = 0.0f; // White Texture
		constexpr float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (sData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			flushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVBOPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVBOPtr->Color = color;
			sData.QuadVBOPtr->TexCoord = textureCoords[i];
			sData.QuadVBOPtr->TextureIndex = textureIndex;
			sData.QuadVBOPtr->TilingFactor = tilingFactor;
			sData.QuadVBOPtr++;
		}

		sData.QuadIndexCount += 6;
		sData.Info.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subTexture->getTextureCoords();
		const std::shared_ptr<Texture2D> texture = subTexture->getTexture();

		if (sData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			flushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < sData.TextureSlotIndex; i++)
		{
			if (*sData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = texture;
			sData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVBOPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVBOPtr->Color = color;
			sData.QuadVBOPtr->TexCoord = textureCoords[i];
			sData.QuadVBOPtr->TextureIndex = textureIndex;
			sData.QuadVBOPtr->TilingFactor = tilingFactor;
			sData.QuadVBOPtr++;
		}

		sData.QuadIndexCount += 6;
		sData.Info.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (sData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			flushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < sData.TextureSlotIndex; i++)
		{
			if (*sData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = texture;
			sData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVBOPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVBOPtr->Color = color;
			sData.QuadVBOPtr->TexCoord = textureCoords[i];
			sData.QuadVBOPtr->TextureIndex = textureIndex;
			sData.QuadVBOPtr->TilingFactor = tilingFactor;
			sData.QuadVBOPtr++;
		}

		sData.QuadIndexCount += 6;
		sData.Info.QuadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		constexpr float textureIndex = 0.0f; // White Texture
		constexpr float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (sData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			flushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVBOPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVBOPtr->Color = color;
			sData.QuadVBOPtr->TexCoord = textureCoords[i];
			sData.QuadVBOPtr->TextureIndex = textureIndex;
			sData.QuadVBOPtr->TilingFactor = tilingFactor;
			sData.QuadVBOPtr++;
		}

		sData.QuadIndexCount += 6;
		sData.Info.QuadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (sData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			flushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < sData.TextureSlotIndex; i++)
		{
			if (*sData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = texture;
			sData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVBOPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVBOPtr->Color = color;
			sData.QuadVBOPtr->TexCoord = textureCoords[i];
			sData.QuadVBOPtr->TextureIndex = textureIndex;
			sData.QuadVBOPtr->TilingFactor = tilingFactor;
			sData.QuadVBOPtr++;
		}

		sData.QuadIndexCount += 6;
		sData.Info.QuadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subTexture->getTextureCoords();
		const std::shared_ptr<Texture2D> texture = subTexture->getTexture();

		if (sData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			flushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < sData.TextureSlotIndex; i++)
		{
			if (*sData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = texture;
			sData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVBOPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVBOPtr->Color = color;
			sData.QuadVBOPtr->TexCoord = textureCoords[i];
			sData.QuadVBOPtr->TextureIndex = textureIndex;
			sData.QuadVBOPtr->TilingFactor = tilingFactor;
			sData.QuadVBOPtr++;
		}

		sData.QuadIndexCount += 6;
		sData.Info.QuadCount++;
	}

	void Renderer2D::resetStats(void)
	{
		memset(&sData.Info, 0, sizeof(Stats));
	}

	Renderer2D::Stats Renderer2D::getStats(void)
	{
		return sData.Info;
	}

	void Renderer2D::shutdown(void)
	{
	}
}