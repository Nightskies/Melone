#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Melone
{
	class SubTexture2D
	{
	private:
		std::shared_ptr<Texture2D> mTexture;

		glm::vec2 mTextureCoords[4];
	public:
		SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const glm::vec2* getTextureCoords(void) const { return mTextureCoords; }
		const std::shared_ptr<Texture2D> getTexture(void) const { return mTexture; }

		static std::shared_ptr<SubTexture2D> createFromCoords(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& ceilSize, const glm::vec2& spriteSize = { 1, 1 });
	};
}