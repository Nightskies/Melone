#include "../ExampleLayer.h"

#include <Melone/Core/EntryPoint.h>

class Sandbox : public Melone::App
{
public:
	Sandbox(void)
	{
		pushLayer(new ExampleLayer());
	}

	~Sandbox(void) = default;
};

Melone::App* Melone::createApp(void)
{
	return new Sandbox;
}