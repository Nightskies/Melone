#pragma once
#include "Texture.h"

#include <glm/glm.hpp>

namespace Melone
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const SPtr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		~SubTexture2D() = default;

		const glm::vec2* GetTextureCoords() const { return mTextureCoords; }
		const SPtr<Texture2D> GetTexture() const { return mTexture; }

		static SPtr<SubTexture2D> CreateFromCoords(const SPtr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& ceilSize, const glm::vec2& spriteSize = { 1, 1 });
	private:
		SPtr<Texture2D> mTexture;

		glm::vec2 mTextureCoords[4];
	};
}