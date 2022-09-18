#include "mlpch.h"
#include "Renderer2D.h"

#include "ArrayObj.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "UniformBuffer.h"

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

		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int EntityID;
	};

	static struct Renderer2DStash
	{
		static const unsigned int MaxQuads = 1000;
		static const unsigned int MaxVertices = MaxQuads * 4;
		static const unsigned int MaxIndices = MaxQuads * 6;
		static const unsigned int MaxTextureSlots = 32;

		SPtr<Texture2D> WhiteTexture;

		SPtr<VAO> QuadVAO;
		SPtr<VBO> QuadVBO;
		SPtr<Shader> QuadShader;

		SPtr<VAO> CircleVAO;
		SPtr<VBO> CircleVBO;
		SPtr<Shader> CircleShader;

		SPtr<VAO> LineVAO;
		SPtr<VBO> LineVBO;
		SPtr<Shader> LineShader;

		unsigned int QuadIndexCount = 0;
		QuadVertex* QuadVBOBase = nullptr;
		QuadVertex* QuadVBOPtr = nullptr;

		unsigned int CircleIndexCount = 0;
		CircleVertex* CircleVBOBase = nullptr;
		CircleVertex* CircleVBOPtr = nullptr;

		unsigned int LineVertexCount = 0;
		LineVertex* LineVBOBase = nullptr;
		LineVertex* LineVBOPtr = nullptr;

		float LineWidth = 2.0f;

		std::array<SPtr<Texture2D>, MaxTextureSlots> TextureSlots;
		unsigned int TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		SPtr<UniformBuffer> CameraUniformBuffer;

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
			{ ShaderDataType::Float, "aTilingFactor" },
			{ ShaderDataType::Int, "aEntityID" }
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

		SPtr<IBO> quadIBO = IBO::Create(quadIndices.data(), Renderer2DData.MaxIndices);
		Renderer2DData.QuadVAO->SetIBO(quadIBO);

		Renderer2DData.CircleVAO = VAO::Create();

		Renderer2DData.CircleVBO = VBO::Create(Renderer2DData.MaxVertices * sizeof(CircleVertex));

		Renderer2DData.CircleVBO->SetLayout({
			{ ShaderDataType::Float3, "aWorldPosition" },
			{ ShaderDataType::Float3, "aLocalPosition" },
			{ ShaderDataType::Float4, "aColor"         },
			{ ShaderDataType::Float,  "aThickness"     },
			{ ShaderDataType::Float,  "aFade"          },
			{ ShaderDataType::Int,    "aEntityID"      }
			});
		Renderer2DData.CircleVAO->AddVBO(Renderer2DData.CircleVBO);

		Renderer2DData.CircleVAO->SetIBO(quadIBO); // Use quad IB
		Renderer2DData.CircleVBOBase = new CircleVertex[Renderer2DData.MaxVertices];

		Renderer2DData.LineVAO = VAO::Create();

		Renderer2DData.LineVBO = VBO::Create(Renderer2DData.MaxVertices * sizeof(LineVertex));

		Renderer2DData.LineVBO->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor"    },
			{ ShaderDataType::Int,    "aEntityID" }
			});
		Renderer2DData.LineVAO->AddVBO(Renderer2DData.LineVBO);
		Renderer2DData.LineVBOBase = new LineVertex[Renderer2DData.MaxVertices];

		Renderer2DData.WhiteTexture = Texture2D::Create(1, 1);
		unsigned int whiteTextureData = 0xffffffff;
		Renderer2DData.WhiteTexture->SetData(&whiteTextureData, sizeof(unsigned int));

		int samplers[Renderer2DData.MaxTextureSlots];
		for (unsigned int i = 0; i < Renderer2DData.MaxTextureSlots; i++)
			samplers[i] = i;

		Renderer2DData.QuadShader = Shader::Create("Assets/Shaders/Renderer2D_Quad.glsl");
		Renderer2DData.CircleShader = Shader::Create("Assets/Shaders/Renderer2D_Circle.glsl");
		Renderer2DData.LineShader = Shader::Create("Assets/Shaders/Renderer2D_Line.glsl");

		Renderer2DData.TextureSlots[0] = Renderer2DData.WhiteTexture;

		Renderer2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Renderer2DData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		Renderer2DData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		Renderer2DData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		Renderer2DData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStash::CameraData), 0);
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		Renderer2DData.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		Renderer2DData.CameraUniformBuffer->SetData(&Renderer2DData.CameraBuffer, sizeof(Renderer2DStash::CameraData));

		Renderer2DData.QuadIndexCount = 0;
		Renderer2DData.QuadVBOPtr = Renderer2DData.QuadVBOBase;

		Renderer2DData.CircleIndexCount = 0;
		Renderer2DData.CircleVBOPtr = Renderer2DData.CircleVBOBase;

		Renderer2DData.LineVertexCount = 0;
		Renderer2DData.LineVBOPtr = Renderer2DData.LineVBOBase;

		Renderer2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		Renderer2DData.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		Renderer2DData.CameraUniformBuffer->SetData(&Renderer2DData.CameraBuffer, sizeof(Renderer2DStash::CameraData));

		Renderer2DData.QuadIndexCount = 0;
		Renderer2DData.QuadVBOPtr = Renderer2DData.QuadVBOBase;

		Renderer2DData.CircleIndexCount = 0;
		Renderer2DData.CircleVBOPtr = Renderer2DData.CircleVBOBase;

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
		if (Renderer2DData.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)Renderer2DData.QuadVBOPtr - (uint8_t*)Renderer2DData.QuadVBOBase);
			Renderer2DData.QuadVBO->SetData(Renderer2DData.QuadVBOBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < Renderer2DData.TextureSlotIndex; i++)
				Renderer2DData.TextureSlots[i]->Bind(i);

			Renderer2DData.QuadShader->Bind();
			RenderCommand::DrawIndexed(Renderer2DData.QuadVAO, Renderer2DData.QuadIndexCount);
			Renderer2DData.Info.DrawCalls++;
		}

		if (Renderer2DData.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)Renderer2DData.CircleVBOPtr - (uint8_t*)Renderer2DData.CircleVBOBase);
			Renderer2DData.CircleVBO->SetData(Renderer2DData.CircleVBOBase, dataSize);

			Renderer2DData.CircleShader->Bind();
			RenderCommand::DrawIndexed(Renderer2DData.CircleVAO, Renderer2DData.CircleIndexCount);
			Renderer2DData.Info.DrawCalls++;
		}

		if (Renderer2DData.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)Renderer2DData.LineVBOPtr - (uint8_t*)Renderer2DData.LineVBOBase);
			Renderer2DData.LineVBO->SetData(Renderer2DData.LineVBOBase, dataSize);

			Renderer2DData.LineShader->Bind();
			RenderCommand::SetLineWidth(Renderer2DData.LineWidth);
			RenderCommand::DrawLines(Renderer2DData.LineVAO, Renderer2DData.LineVertexCount);
			Renderer2DData.Info.DrawCalls++;
		}
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		Renderer2DData.QuadIndexCount = 0;
		Renderer2DData.QuadVBOPtr = Renderer2DData.QuadVBOBase;

		Renderer2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		for (size_t i = 0; i < 4; i++)
		{
			Renderer2DData.CircleVBOPtr->WorldPosition = transform * Renderer2DData.QuadVertexPositions[i];
			Renderer2DData.CircleVBOPtr->LocalPosition = Renderer2DData.QuadVertexPositions[i] * 2.0f;
			Renderer2DData.CircleVBOPtr->Color = color;
			Renderer2DData.CircleVBOPtr->Thickness = thickness;
			Renderer2DData.CircleVBOPtr->Fade = fade;
			Renderer2DData.CircleVBOPtr->EntityID = entityID;
			Renderer2DData.CircleVBOPtr++;
		}

		Renderer2DData.CircleIndexCount += 6;

		Renderer2DData.Info.QuadCount++;
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

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
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
			Renderer2DData.QuadVBOPtr->EntityID = entityID;
			Renderer2DData.QuadVBOPtr++;
		}

		Renderer2DData.QuadIndexCount += 6;
		Renderer2DData.Info.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
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
			Renderer2DData.QuadVBOPtr->EntityID = entityID;
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

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}
	
	void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		Renderer2DData.LineVBOPtr->Position = p0;
		Renderer2DData.LineVBOPtr->Color = color;
		Renderer2DData.LineVBOPtr->EntityID = entityID;
		Renderer2DData.LineVBOPtr++;

		Renderer2DData.LineVBOPtr->Position = p1;
		Renderer2DData.LineVBOPtr->Color = color;
		Renderer2DData.LineVBOPtr->EntityID = entityID;
		Renderer2DData.LineVBOPtr++;

		Renderer2DData.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * Renderer2DData.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	float Renderer2D::GetLineWidth()
	{
		return Renderer2DData.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		Renderer2DData.LineWidth = width;
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