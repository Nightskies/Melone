#pragma once
#include "Melone/Core/Core.h"
#include "Melone/Events/EventSystem.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

struct GLFWwindow;

namespace Melone
{
	struct WindowProperties
	{
		std::string Title;
		std::pair<int, int> Dimensions;
	};

	class Window
	{
	public:
		using WindowType = std::variant<Windows, Linux>;
	public:
		Window(WindowProperties&& winProps, WindowType&& wType = Windows());
		~Window() = default;

		GLFWwindow* GetHandle() const { return mHandle; }
		const WindowProperties& GetProperties() const { return mProperties; }

		bool IsMinimized() const { return mMinimized; }
		bool IsClosed() const { return mClosed; }
	
		void Update();

		void SetVSync(bool enabled);

		static const Window* GetInstance() { return mInstance; }

		template<typename... Args>
		static UPtr<Window> Create(Args... args) { return std::make_unique<Window>(std::forward<Args>(args)...); }
	private:
		void OnResize(const WindowResizeEvent& e);
		void OnClose(const WindowCloseEvent& e);
	private:
		GLFWwindow* mHandle = nullptr;

		WindowProperties mProperties;

		WindowType mWindowType;
		OpenGLContext mContextType;

		bool mVSync = true;
		bool mMinimized = false;
		bool mClosed = false;

		inline static const Window* mInstance;
	};
}