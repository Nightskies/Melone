#include "mlpch.h"
#include "App.h"

#include "Log.h"
#include "Melone/ImGui/ImGuiLayer.h"

#include "Platform/OpenGL/OpenGLBufferObj.h"

#include <glad/glad.h>

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

		mImGuiLayer = new ImGuiLayer();
		pushOverlay(mImGuiLayer);

		/* Square */
		float vertices[] = {
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 3,
			0, 2, 3
		};


		mVBO.reset(VBO::create(vertices, sizeof(vertices)));
		mVBO->setLayout({
			{ ShaderDataType::Float3, "aPosition" }
			});

		mIBO.reset(IBO::create(indices, sizeof(indices) / sizeof(unsigned int)));

		mVAO = VAO::create();
		mVAO->addVBO(mVBO);
		mVAO->setIBO(mIBO);

		mShader = Shader::create("Assets/Shaders/FirstShader.glsl");
	}

	void App::run(void)
	{
		while (mRunning)
		{
			if (!mMinimized)
			{
				glClear(GL_COLOR_BUFFER_BIT);

				mShader->bind();
				mVAO->bind();
				glDrawElements(GL_TRIANGLES, mIBO->getCount(), GL_UNSIGNED_INT, nullptr);

				for (auto layer : mLayerStack)
					layer->onUpdate();

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