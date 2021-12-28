#pragma once

namespace Melone
{
	class ITexture
	{
	public:
		virtual ~ITexture(void) = default;

		virtual void bind(unsigned int slot = 0) const = 0;

		virtual int getWidth(void) const = 0;
		virtual int getHeight(void) const = 0;

		virtual void setData(void* data, unsigned int size) = 0;

		virtual bool operator==(const ITexture& other) const = 0;
	};

	class Texture2D : public ITexture
	{
	public:
		static std::shared_ptr<Texture2D> create(unsigned int width, unsigned int height);
		static std::shared_ptr<Texture2D> create(const std::string& path);
	};
}
