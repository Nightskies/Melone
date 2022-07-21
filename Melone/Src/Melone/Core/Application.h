#pragma once

#include "Core.h"
#include "Melone/ImGui/GUI.h"
#include "Layer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLArrayObj.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Melone
{
	class Application
	{
	private:
		const Window& mWindow;
		SPtr<GUI> mGUI;

		std::vector<UPtr<Layer>> mLayers;

		Timestep mTimeStep;
	public:
		Application(std::string&& name = "Melone Engine");
		~Application();

		void Run();

		static void Close();
	protected:
		template<typename... Args>
		void AddLayer(Args... args) { mLayers.emplace_back(std::forward<Args>(args)...)->OnAttach(); }

		void RemoveLayer(const UPtr<Layer>& layer);
	private:
		const Timestep& TimeStepUpdate();
	};

	Application* CreateApplication();
}