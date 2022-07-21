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

	static struct Renderer2DStash
	{
		static const unsigned int MaxQuads = 1000;
		static const unsigned int MaxVertices = MaxQuads * 4;
		static const unsigned int MaxIndices = MaxQuads * 6;
		static const unsigned int MaxTextureSlots = 32;

		SPtr<VAO> QuadVAO;
		SPtr<VBO> QuadVBO;
		SPtr<Shader> TextureShader;
		SPtr<Texture2D> WhiteTexture;

		unsigned int QuadIndexCount = 0;
		QuadVertex* QuadVBOBase = nullptr;
		QuadVertex* QuadVBOPtr = nullptr;

		std::array<SPtr<Texture2D>, MaxTextureSlots> TextureSlots;
		unsigned int TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Stats Info;
	} Renderer2DData;

	void Renderer2D::Init()
	{
		Renderer2DData.QuadVAO = VAO::Create();

		Renderer2DData.QuadVBO = VBO::Create(Renderer2DData.MaxVertices * sizeof(QuadVertex));

		Renderer2DData.QuadVBO->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float2, "aTexCoord" },
			{ ShaderDataType::Float, "aTexIndex"},
			{ ShaderDataType::Float, "aTilingFactor" }
			});

		Renderer2DData.QuadVAO->AddVBO(Renderer2DData.QuadVBO);

		Renderer2DData.QuadVBOBase = new QuadVertex[Renderer2DData.MaxVertices];

		std::vector<unsigned int> quadIndices(Renderer2DData.MaxIndices);

		unsigned int offset = 0;
		for (size_t i = 0; i < Renderer2DData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		SPtr<IBO> squareIBO = IBO::Create(quadIndices.data(), Renderer2DData.MaxIndices);
		Renderer2DData.QuadVAO->SetIBO(squareIBO);

		Renderer2DData.WhiteTexture = Texture2D::Create(1, 1);
		unsigned int whiteTextureData = 0xffffffff;
		Renderer2DData.WhiteTexture->SetData(&whiteTextureData, sizeof(unsigned int));

		int samplers[Renderer2DData.MaxTextureSlots];
		for (unsigned int i = 0; i < Renderer2DData.MaxTextureSlots; i++)
			samplers[i] = i;

		Renderer2DData.TextureShader = Shader::Create("Assets/Shaders/TextureShader.glsl");
		Renderer2DData.TextureShader->Bind();
		Renderer2DData.TextureShader->SetUniformIntArray("uTextures", samplers, Renderer2DData.MaxTextureSlots);

		Renderer2DData.TextureSlots[0] = Renderer2DData.WhiteTexture;

		Renderer2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Renderer2DData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		Renderer2DData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		Renderer2DData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		Renderer2DData.TextureShader->Bind();
		Renderer2DData.TextureShader->SetUniformMat4("uViewProjection", camera.GetViewProjectionMatrix());

		Renderer2DData.QuadIndexCount = 0;
		Renderer2DData.QuadVBOPtr = Renderer2DData.QuadVBOBase;

		Renderer2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		Renderer2DData.TextureShader->Bind();
		Renderer2DData.TextureShader->SetUniformMat4("uViewProjection", viewProj);

		Renderer2DData.QuadIndexCount = 0;
		Renderer2DData.QuadVBOPtr = Renderer2DData.QuadVBOBase;

		Renderer2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		unsigned int dataSize = (uint8_t*)Renderer2DData.QuadVBOPtr - (uint8_t*)Renderer2DData.QuadVBOBase;
		Renderer2DData.QuadVBO->SetData(Renderer2DData.QuadVBOBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (unsigned int i = 0; i < Renderer2DData.TextureSlotIndex; i++)
			Renderer2DData.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(Renderer2DData.QuadVAO, Renderer2DData.QuadIndexCount);
		Renderer2DData.Info.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		Renderer2DData.QuadIndexCount = 0;
		Renderer2DData.QuadVBOPtr = Renderer2DData.QuadVBOBase;

		Renderer2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		constexpr float textureIndex = 0.0f; // White Texture
		constexpr float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();
		const std::shared_ptr<Texture2D> texture = subTexture->GetTexture();

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < Renderer2DData.TextureSlotIndex; i++)
		{
			if (*Renderer2DData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Renderer2DData.TextureSlotIndex;
			Renderer2DData.TextureSlots[Renderer2DData.TextureSlotIndex] = texture;
			Renderer2DData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < Renderer2DData.TextureSlotIndex; i++)
		{
			if (*Renderer2DData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Renderer2DData.TextureSlotIndex;
			Renderer2DData.TextureSlots[Renderer2DData.TextureSlotIndex] = texture;
			Renderer2DData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < Renderer2DData.TextureSlotIndex; i++)
		{
			if (*Renderer2DData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Renderer2DData.TextureSlotIndex;
			Renderer2DData.TextureSlots[Renderer2DData.TextureSlotIndex] = texture;
			Renderer2DData.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		constexpr float textureIndex = 0.0f; // White Texture
		constexpr float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < Renderer2DData.TextureSlotIndex; i++)
		{
			if (*Renderer2DData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Renderer2DData.TextureSlotIndex;
			Renderer2DData.TextureSlots[Renderer2DData.TextureSlotIndex] = texture;
			Renderer2DData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();
		const std::shared_ptr<Texture2D> texture = subTexture->GetTexture();

		if (Renderer2DData.QuadIndexCount >= Renderer2DStash::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < Renderer2DData.TextureSlotIndex; i++)
		{
			if (*Renderer2DData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Renderer2DData.TextureSlotIndex;
			Renderer2DData.TextureSlots[Renderer2DData.TextureSlotIndex] = texture;
			Renderer2DData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Renderer2DData.QuadVBOPtr->Position = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.QuadVBOPtr->Color = color;
			Renderer2DData.QuadVBOPtr->TexCoord = textureCoords[i];
			Renderer2DData.QuadVBOPtr->TextureIndex = textureIndex;
			Renderer2DData.QuadVBOPtr->TilingFactor = tilingFactor;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&Renderer2DData.Info, 0, sizeof(Stats));
	}

	Renderer2D::Stats Renderer2D::GetStats()
	{
		return Renderer2DData.Info;
	}

	void Renderer2D::Shutdown()
	{
	}
}