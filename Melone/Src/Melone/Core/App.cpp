#include "App.h"

#include "Log.h"
#include "Melone/ImGui/ImGuiLayer.h"

namespace Melone
{
#define BIND_EVENT_FN(x) std::bind(&App::x, this, std::placeholders::_1)

	App* App::sInstance;

	App::App(void)
	{
		MELONE_CORE_ASSERT(!sInstance, "App already exists!");
		sInstance = this;

		mWindow = std::make_unique<Window>(800, 600);
		mWindow->setEventCallback(BIND_EVENT_FN(onEvent));

		mLayer = new ImGuiLayer();
		pushOverlay(mLayer);
	}

	void App::run(void)
	{
		while (mRunning)
		{
			if (!mMinimized)
			{
				for (auto layer : mLayerStack)
					layer->onUpdate();
			}
			mWindow->update();
		}
	}
	
	void App::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
			(*--it)->onEvent(e);
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

	void App::pushLayer(Layer* layer)
	{
		mLayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void App::pushOverlay(Layer* layer)
	{
		mLayerStack.pushOverlay(layer);
		layer->onAttach();
	}
}