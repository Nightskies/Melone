#include "mlpch.h"
#include "App.h"

#include "Log.h"

#include "Melone/ImGui/ImGuiLayer.h"
#include "Melone/Renderer/Renderer.h"

#include <glfw/glfw3.h>

namespace Melone
{
#define BIND_EVENT_FN(x) std::bind(&App::x, this, std::placeholders::_1)

	App* App::sInstance;

	App::App(const std::string& name)
	{
		MELONE_CORE_ASSERT(!sInstance, "App already exists!");
		sInstance = this;

		mWindow = std::make_unique<Window>(name, 1280, 720);
		mWindow->setEventCallback(BIND_EVENT_FN(onEvent));

		Renderer::init();

		mImGuiLayer = new ImGuiLayer();
		pushOverlay(mImGuiLayer);
	}

	void App::run(void)
	{
		while (mRunning)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - mLastFrameTime;
			mLastFrameTime = time;

			if (!mMinimized)
			{
				for (auto layer : mLayerStack)
					layer->onUpdate(timestep);

				mImGuiLayer->begin();
				for (auto layer : mLayerStack)
					layer->onImGuiRender();
				mImGuiLayer->end();
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
		{
			if (e.handled)
				break;
			(*--it)->onEvent(e);
		}
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
		Renderer::onWindowResize(e.getWinDimensions());

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

	void App::close(void)
	{
		mRunning = false;
	}

	App::~App(void)
	{
		Renderer::shutdown();
	}
}