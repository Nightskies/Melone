#include "App.h"

#include "Log.h"

namespace Melone
{
#define BIND_EVENT_FN(x) std::bind(&App::x, this, std::placeholders::_1)

	App::App(void)
	{
		mWindow = std::make_unique<Window>(800, 600);
		mWindow->setEventCallback(BIND_EVENT_FN(onEvent));
	}

	void App::run(void)
	{
		while (mRunning)
		{
			if (!mMinimized)
				mWindow->updateImGui();
			mWindow->update();
		}
	}
	
	void App::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

		MELONE_CORE_INFO("{0}", e.toString());
	}

	bool App::onWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool App::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWinDimensions().first == 0 || e.getWinDimensions().second == 0)
		{
			mMinimized = true;
			return false;
		}

		mMinimized = false;
		mWindow->setViewport(e.getWinDimensions());

		return false;
	}
}