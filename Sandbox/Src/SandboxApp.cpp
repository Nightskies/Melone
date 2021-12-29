#include "ExampleLayer.h"

#include "Sandbox2D.h"

#include <Melone/Core/EntryPoint.h>

class Sandbox : public Melone::App
{
public:
	Sandbox(void)
	{
		pushLayer(new Sandbox2D());
	}

	~Sandbox(void) = default;
};

Melone::App* Melone::createApp(void)
{
	return new Sandbox;
}