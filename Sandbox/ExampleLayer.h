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

		if (e.getEventType() == Melone::EventType::KeyPressed)
		{
			Melone::KeyPressedEvent& ev = static_cast<Melone::KeyPressedEvent&>(e);

			if (ev.getKeyCode() == MELONE_KEY_TAB)
				MELONE_TRACE("Tab key is pressed (event)!");

			MELONE_TRACE("{0}", (char)ev.getKeyCode());
		}
	}

	void onUpdate(void) override
	{
		if (Melone::Input::isKeyPressed(MELONE_KEY_TAB))
			MELONE_TRACE("Tab key is pressed (poll)!");

		//MELONE_TRACE("{0} layer update!", mName);
	}
};