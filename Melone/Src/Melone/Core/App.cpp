#include "App.h"

namespace Melone
{
	App::App(void)
	{
		mWindow = std::make_unique<Window>();
	}

	void App::run(void)
	{
		while (true)
			mWindow->update();
	}
}