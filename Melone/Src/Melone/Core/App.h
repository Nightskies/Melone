#pragma once

#include "Core.h"
#include "Window.h"
#include "Melone/Events/WindowEvent.h"
#include "LayerStack.h"

#include <functional>

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
		
		static App* sInstance;
	public:
		App(void);
		virtual ~App(void) = default;
		App(const App&) = delete;
		App& operator=(App&) = delete;

		const App& getApp() const { return *this; }

		static App& getInstance(void) { return *sInstance; }
		Window& getWindow(void) const { return *mWindow; }
	
		void run(void);

		void onEvent(Event& e);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
	};

	App* createApp(void);
}
