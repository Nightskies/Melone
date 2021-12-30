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
		bool mVSync;
	public:
		Window(const std::string& title, int width = 1280, int height = 720);

		~Window() = default;

		const std::pair<int32_t, int32_t>& getWinDimentions() const { return mWinDimensions; }
		GLFWwindow* getNativeWindow(void) { return mWindow; }

		void setWinDimentions(std::pair<int32_t, int32_t> winDimentions) { mWinDimensions = std::move(winDimentions); }
		void setEventCallback(const EventCallback& callback) { mEventCallback = callback; }
		void setVSync(bool enabled);

		void shutdown(void);
		void update(void);
	};
}