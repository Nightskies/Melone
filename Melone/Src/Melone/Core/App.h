#pragma once

#include "Core.h"

#include "Window.h"

namespace Melone
{
	class App
	{
	private:
		std::unique_ptr<Window> mWindow;
	public:
		App(void);
		virtual ~App(void) = default;
	
		void run(void);
	};

	App* createApp(void);
}
