#pragma once

#include "Platform/OpenGL/OpenGLContext.h"

#include "Melone/Events/Event.h"

struct GLFWwindow;

namespace Melone
{
	class Window final
	{
	private:
		GLFWwindow* mWindow;
		IGraphicsContext* mContext;

		using EventCallback = std::function<void(Event&)>;
		EventCallback mEventCallback;

		// Width and Height
		std::pair<int, int> mWinDimensions;
		std::string mTitle;
	public:
		Window(int32_t width = 1280, int32_t height = 720, std::string title = "Melone Engine");

		~Window() = default;

		const std::pair<int32_t, int32_t>& getWinDimentions() const { return mWinDimensions; }
		GLFWwindow* getNativeWindow(void) { return mWindow; }

		void setWinDimentions(std::pair<int32_t, int32_t> winDimentions) { mWinDimensions = std::move(winDimentions); }
		void setEventCallback(const EventCallback& callback) { mEventCallback = callback; }

		void shutdown(void);
		void update(void);

		void setViewport(const std::pair<int32_t, int32_t>& winDimentions);
		void setClearColor(float r = 1.0f, float g = 0.0f, float b = 1.0f, float a = 1.0f);
	};
}