#pragma once
#include "EventPublisher.h"

namespace Fiea::GameEngine {

	template <typename EventType>
	class Event : public EventPublisher {
		RTTI_DECLARATIONS(Event<EventType>, EventPublisher);

	public:

		// Constructor – Takes a reference to a message object(the templated T) and a boolean indicating whether to delete the event after publishing it.
		Event(EventType Message, bool deleteOnPublish);

		// Copy Semantics
		Event(const Event& other);

		Event& operator=(const Event& rhs);
		// Move Semantics
		Event(Event&& other);

		Event& operator=(Event&& rhs);

		virtual ~Event() {};

		// Subscribe – (static) Given the address of an EventSubscriber, add it to the list of subscribers for this event type
		static void Subscribe(EventSubscriber* subscriber) {
			m_subscribers.push_back(subscriber);
		}

		// Unsubscribe – (static) Given the address of an EventSubscriber, remove it from the list of subscribers for this event type.
		static void Unsubscribe(EventSubscriber* unsubscriber) {
			for (const auto& sub : m_subscribers) {
				if (sub == unsubscriber) {
					m_subscribers.erase(sub);
				}
			}
		}

		// UnsubscribeAll – (static)  Unsubscribe all subscribers to this event type.
		static void UnsubscribeAll() {
			m_subscribers.clear();
		}

		// Message – returns message object.
		EventType& Message();

	private:
		EventType m_Message; // Payload
		std::vector<EventSubscriber*> m_eventSubscribers; // static vector of subscribers so any EventSubscriber could easily subscribe to the evnt
	};

}

#include "Event.inl"