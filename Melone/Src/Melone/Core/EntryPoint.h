#pragma once

#ifdef MELONE_PLATFORM_WINDOWS

extern Melone::App* Melone::createApp();

int main(int argc, char** argv)
{
	auto app = Melone::createApp();
	app->run();
	delete app;
}

#endif