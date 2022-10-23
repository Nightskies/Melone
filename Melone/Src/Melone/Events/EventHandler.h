#pragma once
#include "Event.h"

namespace Melone
{
	namespace Event
	{
		template<class T, class E>
		using EventCallback = void (T::*)(const E&);

		class EventHandlerBase
		{
		public:
			EventHandlerBase() = default;
			virtual ~EventHandlerBase() = default;

			void Invoke(const Events& e)
			{
				Call(e);
			}
		private:
			virtual void Call(const Events& e) = 0;
		};

		using HandlerList = std::list<SPtr<EventHandlerBase>>;

		template<class T, class E>
		class EventHandler : public EventHandlerBase
		{
		public:
			EventHandler(T* instance, EventCallback<T, E> callback)
				:
				mInstance(instance),
				mCallback(callback)
			{}

			~EventHandler() override = default;

			void Call(const Events& events) override
			{
				const auto& e = std::get<E>(events);

				MELONE_CORE_INFO(e.ToString());

				(mInstance->*mCallback)(e);
			}
		private:
			T* mInstance;

			EventCallback<T, E> mCallback;
		};
	}
}