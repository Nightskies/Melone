#include "App.h"

namespace Melone
{
	App::App(void)
	{
		mWindow = std::make_unique<Window>(800, 600);
	}

	void App::run(void)
	{
		while (true)
		{
			mWindow->updateImGui();
			mWindow->update();
		}

	}
}