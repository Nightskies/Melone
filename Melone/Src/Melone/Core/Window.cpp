#include "Window.h"

#include "Log.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"


#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Melone
{
	Window::Window(int32_t width, int32_t height, std::string title)
		:
		mTitle(std::move(title)),
		mHeight(height),
		mWidth(width)
	{
		int success = glfwInit();
		MELONE_CORE_ASSERT(success, "Could't initialize GLFW!");

		mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
		MELONE_CORE_ASSERT(mWindow, "Failed create window!");

		MELONE_CORE_INFO("Created window {0} ({1}, {2})", mTitle, mWidth, mHeight);

		mContext = new OpenGLContext(mWindow);
		mContext->init();

		setViewport(mWidth, mHeight);
		setClearColor();
		initImGui();
	}

	void Window::initImGui(void)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		 // Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void Window::updateImGui(void)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool show_demo_window = true;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)mWidth, (float)mHeight);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Window::shutdown(void)
	{
		glfwDestroyWindow(mWindow);
	}

	void Window::update(void)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		mContext->swapBuffers();
		glfwPollEvents();
	}

	void Window::setViewport(int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Window::setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
}