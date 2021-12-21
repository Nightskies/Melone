#pragma once

#include "mlpch.h"

namespace Melone
{
	enum class EventType
	{
		Undefined,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	class Event
	{
	public:
		virtual ~Event(void) = default;

		virtual EventType getEventType(void) const = 0;
		virtual const char* getName() const = 0;
		virtual std::string toString(void) const { return getName(); }
	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }\
	virtual EventType getEventType() const override { return getStaticType(); }\
	virtual const char* getName() const override { return #type; }

	class EventDispatcher
	{
	private:
		Event& mEvent;

		template<typename T>
		using EventFunc = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: 
			mEvent(event)
		{}

		~EventDispatcher(void) = default;

		template<typename T>
		bool dispatch(EventFunc<T> func)
		{
			if (mEvent.getEventType() == T::getStaticType())
			{
				func(*(static_cast<T*>(&mEvent)));
				return true;
			}
			return false;
		}
	};
}