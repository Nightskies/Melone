#include <Melone.h>

class Sandbox : public Melone::App
{
public:
	Sandbox(void) = default;
	~Sandbox(void) = default;
};

Melone::App* Melone::createApp(void)
{
	return new Sandbox;
}