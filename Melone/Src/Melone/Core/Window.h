#pragma once
#include "Melone/Core/Core.h"

#include "Melone/Events/EventSystem.h"
#include "Melone/Core/Timestep.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <optional>

struct GLFWwindow;

namespace Melone
{
	struct WindowProperties
	{
		std::string mTitle;
		std::pair<int, int> mDimensions;
	};

	class Window
	{
	private:
		GLFWwindow* mNativeWindow;

		WindowProperties mProperties;

		using Windows = WindowsWindow;
		using Linux = LinuxWindow;
		std::variant<Windows, Linux> mWindowType;
		
		std::variant<OpenGLContext> mContextType;

		bool mVSync = true;
		bool mMinimized = false;
		bool mClosed = false;	
	public:
		Window(WindowProperties&& winProps);
		~Window() = default;

		GLFWwindow* GetNativeWindow() const { return mNativeWindow; }
		const WindowProperties& GetProperties() const { return mProperties; }

		bool IsMinimized() const { return mMinimized; }
		bool IsClosed() const { return mClosed; }
	
		void Update() const;

		void SetVSync(bool enabled);

		static const Window& GetInstance(std::optional<WindowProperties>&& winProps = std::nullopt);
	private:
		void OnResize(const WindowResizeEvent& e);
		void OnClose(const WindowCloseEvent& e);
	};
}