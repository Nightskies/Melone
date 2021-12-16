#pragma once

#include "Melone/Renderer/GraphicsContext.h"

#include <string>

struct GLFWwindow;

namespace Melone
{
	class Window
	{
	private:
		GLFWwindow* mWindow;
		IGraphicsContext* mContext;

		int32_t mWidth;
		int32_t mHeight;
		std::string mTitle;
	public:
		Window(int32_t width = 1280, int32_t height = 720, std::string title = "Melone Engine");

		~Window() = default;

		int32_t getHeight(void) const { return mHeight; }
		int32_t getWidth(void) const { return mWidth; }

		void shutdown(void);
		void update(void);

		void setViewport(int32_t width, int32_t height);
		void setClearColor(float r = 1.0f, float g = 0.0f, float b = 1.0f, float a = 1.0f);
	};
}