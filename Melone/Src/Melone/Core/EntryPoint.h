#pragma once

#ifdef MELONE_PLATFORM_WINDOWS

	extern Melone::Application* Melone::CreateApplication();

	int main(int argc, char** argv)
	{
		Melone::Log::Init();

		Melone::Application* app = Melone::CreateApplication();
		app->Run();
		delete app;
	}

#endif