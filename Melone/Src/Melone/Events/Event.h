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

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags(void) const override { return category; }

	class Event
	{
	public:
		bool handled = false;
	public:
		virtual ~Event(void) = default;

		virtual EventType getEventType(void) const = 0;
		virtual const char* getName(void) const = 0;
		virtual int getCategoryFlags(void) const = 0;
		virtual std::string toString(void) const { return getName(); }

		bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
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
				mEvent.handled = func(*(static_cast<T*>(&mEvent)));
				return true;
			}
			return false;
		}

		friend std::ostream& operator<<(std::ostream& os, const Event& e)
		{
			return os << e.toString();
		}
	};
}