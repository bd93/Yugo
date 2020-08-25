#pragma once
#include "Core.h"
#include "Events/Event.h"


namespace Yugo
{

	/*
	This class represents event subscriber and it uses type erasure technique
	in order to store any type of subscriber inside subscriber's list container.
	*/
	class Listener
	{
	private:
		struct ListenerInterface
		{
			virtual ~ListenerInterface() {};
			virtual void OnEvent(const Event& event) = 0;
		};
		
		template<typename T>
		struct ListenerModel : public ListenerInterface
		{
			ListenerModel() = default;
			ListenerModel(T* listener) : Listener(listener) {}
			virtual ~ListenerModel() {}
			virtual void OnEvent(const Event& event) override { Listener->OnEvent(event); }

		private:
			T* Listener;
		};

		sPtr<ListenerInterface> m_Listener;

	public:
		template<typename T>
		Listener(T* listener) { m_Listener = sPtrCreate<ListenerModel<T>>(listener); }
		void OnEvent(const Event& event) { m_Listener->OnEvent(event); }
	};


	/*
	This class is event dispatcher class and it dispatches events to subscribers, 
	when method Publish(const EventType& event) is invoked.
	List of subscribers is stored in std::map, with hash key of type std::type_index
	*/
	class Dispatcher
	{
	public:
		using ListenerList = std::list<sPtr<Listener>>;

		template<typename EventType, typename Subscriber>
		static void Subscribe(Subscriber* subscriber)
		{
			sPtr<Listener> listener = sPtrCreate<Listener>(subscriber);
			m_Subscribers[typeid(EventType)].push_back(listener);
		}

		template<typename EventType>
		static void Publish(const EventType& event)
		{
			if (m_Subscribers.find(typeid(EventType)) != m_Subscribers.end())
				for (const auto& subscriber : m_Subscribers[typeid(EventType)])
					subscriber->OnEvent(event);
		}

	private:
		static std::map<std::type_index, ListenerList> m_Subscribers;
	};


}