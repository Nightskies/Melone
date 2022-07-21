#pragma once
#include "EventHandler.h"

namespace Melone
{
	class EventSystem
	{
	private:
		inline static std::map<Event::EventType, Event::HandlerList> mSubscribers;

		inline static Event::Events mEvents;

		inline static  int mBlockedCategories = 0;
	public:
		template<size_t I, typename... Args>
		static void Publish(Args&&... args)
		{
			mEvents.emplace<I>(std::forward<Args>(args)...);

			if (EventIsNotBlocked<I>())
			{
				const Event::HandlerList& list = mSubscribers[static_cast<Event::EventType>(I)];

				for (const auto& handler : list)
				{
					handler->Invoke(mEvents);
				}
			}
		}

		template<class T, class E>
		static void Subscribe(T* instance, Event::EventCallback<T, E> callback)
		{
			if constexpr (is_variant_type<E, Event::Events>{})
			{
				Event::HandlerList& list = mSubscribers[Event::GetEventType<Event::Events, E>()];
				
				list.emplace_back(std::make_unique<Event::EventHandler<T, E>>(instance, callback));
			}
		}

		static void BlockEvents(EventCategory category)
		{
			mBlockedCategories |= category;
		}

		static void UnBlockEvents()
		{
			mBlockedCategories = 0;
		}
	private:
		template<size_t I>
		static bool EventIsNotBlocked()
		{
			return !(std::get<I>(mEvents).GetCategory() & mBlockedCategories);
		}
	};
}