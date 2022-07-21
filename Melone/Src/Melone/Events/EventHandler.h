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
			virtual ~EventHandlerBase() = default;
		public:
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
		private:
			T* mInstance;

			EventCallback<T, E> mCallback;
		public:
			EventHandler(T* instance, EventCallback<T, E> callback)
				:
				mInstance(instance),
				mCallback(callback)
			{}

			void Call(const Events& events) override
			{
				const auto& e = std::get<E>(events);

				MELONE_CORE_INFO(e.ToString());

				(mInstance->*mCallback)(e);
			}
		};
	}
}