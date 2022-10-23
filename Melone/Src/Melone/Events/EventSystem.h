#pragma once
#include "EventHandler.h"

namespace Melone
{
	class EventSystem
	{
	public:
		template<size_t I, typename... Args>
		static void Publish(Args&&... args)
		{
			mCurEvent.emplace<I>(std::forward<Args>(args)...);

			if (EventIsNotBlocked<I>())
			{
				const Event::HandlerList& list = mSubscribers[static_cast<Event::EventType>(I)];

				for (const auto& handler : list)
				{
					handler->Invoke(mCurEvent);
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
		EventSystem() = default;
		~EventSystem() = default;

		template<size_t I>
		static bool EventIsNotBlocked()
		{
			return !(std::get<I>(mCurEvent).GetCategory() & mBlockedCategories);
		}
	private:
		inline static std::map<Event::EventType, Event::HandlerList> mSubscribers;

		inline static Event::Events mCurEvent;

		inline static  int mBlockedCategories = 0;
	};
}