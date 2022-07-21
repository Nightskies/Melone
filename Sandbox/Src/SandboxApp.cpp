#include "Sandbox2D.h"

#include <Melone/Core/EntryPoint.h>

class Sandbox : public Melone::Application
{
public:
	Sandbox()
		:
		Melone::Application("Sandbox")
	{
		AddLayer(std::make_unique<Sandbox2D>());
	}

	~Sandbox() = default;
};

Melone::Application* Melone::CreateApplication()
{
	return new Sandbox();
}