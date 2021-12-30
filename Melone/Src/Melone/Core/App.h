#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

#include "Melone/Events/WindowEvent.h"
#include "Melone/ImGui/ImGuiLayer.h"
#include "Melone/Renderer/OrthographicCamera.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLArrayObj.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Melone
{
	class App
	{
	private:
		std::unique_ptr<Window> mWindow;

		bool mRunning = true;
		bool mMinimized = false;

		Layer* mLayer;
		LayerStack mLayerStack;
		ImGuiLayer* mImGuiLayer;

		float mLastFrameTime = 0.0f;
		
		static App* sInstance;
	public:
		App(void);
		virtual ~App(void);
		App(const App&) = delete;
		App& operator=(App&) = delete;

		static App& getInstance(void) { return *sInstance; }
		Window& getWindow(void) const { return *mWindow; }
	
		void run(void);
		void close(void);

		void onEvent(Event& e);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
	};

	App* createApp(void);
}
