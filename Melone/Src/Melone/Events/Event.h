#pragma once
#include "mlpch.h"

#include "KeyEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

namespace Melone
{
	namespace Event
	{
		enum EventType
		{
			KeyPressed = 1,
			KeyReleased,
			KeyTyped,
			MouseMoved,
			MouseScrolled,
			MouseButtonPressed,
			MouseButtonReleased,
			WindowResize,
			WindowClose
		};

		using Events = std::variant<std::monostate,
			KeyPressedEvent,
			KeyReleasedEvent,
			KeyTypedEvent,
			MouseMovedEvent,
			MouseScrolledEvent,
			MouseButtonPressedEvent,
			MouseButtonReleasedEvent,
			WindowResizeEvent,
			WindowCloseEvent>;

		template<typename V, typename E>
		constexpr EventType GetEventType()
		{
			switch (GetVariantIndex<V, E, 1>())
			{
			case 1:
				return KeyPressed;
			case 2:
				return KeyReleased;
			case 3:
				return KeyTyped;
			case 4:
				return MouseMoved;
			case 5:
				return MouseScrolled;
			case 6:
				return MouseButtonPressed;
			case 7:
				return MouseButtonReleased;
			case 8:
				return WindowResize;
			case 9:
				return WindowClose;
			case std::variant_size_v<V>:
				MELONE_CORE_ERROR("Function called with invalid E (event type) or std::monostate");
			}
		}
	}
}