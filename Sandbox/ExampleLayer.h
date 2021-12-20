#pragma once

#include <Melone.h>

class ExampleLayer: public Melone::Layer
{
public:
	ExampleLayer(void)
		:
		Layer("Example")
	{}

	void onEvent(Melone::Event& e) override
	{
		MELONE_INFO("{0}", e.toString());
	}

	void onUpdate(void) override
	{
		MELONE_TRACE("{0} layer update!", mName);
	}
};