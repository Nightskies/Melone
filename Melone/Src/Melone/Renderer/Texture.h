#pragma once
#include "Melone/Core/Core.h"

#include <filesystem>

namespace Melone
{
	class ITexture
	{
	public:
		ITexture() = default;
		virtual ~ITexture() = default;

		virtual void Bind(unsigned int slot = 0) const = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual unsigned int GetRendererID() const = 0;

		virtual void SetData(void* data, unsigned int size) = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const ITexture& other) const = 0;
	};

	class Texture2D : public ITexture
	{
	public:
		Texture2D() = default;
		~Texture2D() override = default;

		static SPtr<Texture2D> Create(unsigned int width, unsigned int height);
		static SPtr<Texture2D> Create(std::filesystem::path&& path);
	};
}
