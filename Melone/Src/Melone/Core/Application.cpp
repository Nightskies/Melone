#include "mlpch.h"
#include "Application.h"

#include "Melone/Renderer/Renderer.h"
#include "Melone/Input/Input.h"

#include <glfw/glfw3.h>

namespace Melone
{
	Application::Application(std::string&& name)
		:
		mWindow(Window::GetInstance({ { std::move(name), { 1280, 720 } } }))
	{	
		mGUI = GUI::Create(mWindow);

		Renderer::Init();
	}

	void Application::Run()
	{
		while (!mWindow.IsClosed())
		{
			if (float ts = TimeStepUpdate(); !mWindow.IsMinimized())
			{
				std::for_each(mLayers.cbegin(), mLayers.cend(), [ts](auto& layer) { layer->OnUpdate(ts); });

				mGUI->Begin();

				std::for_each(mLayers.cbegin(), mLayers.cend(), [](auto& layer) { layer->OnImGuiRender(); });

				mGUI->End();
			}
			mWindow.Update();
		}
	}

	void Application::RemoveLayer(const UPtr<Layer>& layer)
	{
		if (auto& it = std::find(mLayers.begin(), mLayers.end(), layer); it != mLayers.end())
		{
			mLayers.erase(it);
		}
		layer->OnDetach();
	}

	const Timestep& Application::TimeStepUpdate()
	{
		static float lastFrameTime = 0.0f;

		float time = (float)glfwGetTime();
		mTimeStep = time - lastFrameTime;
		lastFrameTime = time;

		return mTimeStep;
	}

	void Application::Close()
	{
		EventSystem::Publish<Event::WindowClose>();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}
}