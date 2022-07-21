#include "mlpch.h"
#include "SubTexture2D.h"

namespace Melone
{
	SubTexture2D::SubTexture2D(const SPtr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		:
		mTexture(texture)
	{
		mTextureCoords[0] = { min.x, min.y };
		mTextureCoords[1] = { max.x, min.y };
		mTextureCoords[2] = { max.x, max.y };
		mTextureCoords[3] = { min.x, max.y };
	}

	SPtr<SubTexture2D> SubTexture2D::CreateFromCoords(const SPtr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& ceilSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (coords.x * ceilSize.x) / texture->GetWidth(), (coords.y * ceilSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * ceilSize.x) / texture->GetWidth(), ((coords.t + spriteSize.y) * ceilSize.y) / texture->GetHeight() };

		return std::make_shared<SubTexture2D>(texture, min, max);
	}
}