#pragma once

#include "Core.h"
#include "Layer.h"

#include "Melone/ImGui/GUI.h"

//#include "Platform/OpenGL/OpenGLShader.h"
//#include "Platform/OpenGL/OpenGLArrayObj.h"
//#include "Platform/OpenGL/OpenGLTexture.h"

namespace Melone
{
	class Application
	{
	public:
		Application(std::string&& name = "Melone Engine");
		~Application();

		void Run();

		static void Close();
	protected:
		template<typename... Args>
		void AddLayer(Args... args) { mLayers.emplace_back(std::forward<Args>(args)...)->OnAttach(); }
	private:
		SPtr<Window> mWindow;
		SPtr<GUI> mGUI;

		std::vector<UPtr<Layer>> mLayers;

		Timestep mTimestep;
	};

	Application* CreateApplication();
}