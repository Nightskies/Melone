#pragma once

#include "Core.h"
#include "Window.h"
#include "Melone/Events/WindowEvent.h"

namespace Melone
{
	class App
	{
	private:
		std::unique_ptr<Window> mWindow;

		bool mRunning = true;
		bool mMinimized = false;
	public:
		App(void);
		virtual ~App(void) = default;
	
		void run(void);

		void onEvent(Event& e);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	};

	App* createApp(void);
}
