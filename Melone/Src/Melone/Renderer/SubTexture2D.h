#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Melone
{
	class SubTexture2D
	{
	private:
		SPtr<Texture2D> mTexture;

		glm::vec2 mTextureCoords[4];
	public:
		SubTexture2D(const SPtr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const glm::vec2* GetTextureCoords() const { return mTextureCoords; }
		const SPtr<Texture2D> GetTexture() const { return mTexture; }

		static SPtr<SubTexture2D> CreateFromCoords(const SPtr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& ceilSize, const glm::vec2& spriteSize = { 1, 1 });
	};
}