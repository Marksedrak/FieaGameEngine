#pragma once
#include "RTTI.h"
#include <vector>
#include "GameClock.h"

namespace Fiea::GameEngine {
	class EventSubscriber;

	class EventPublisher : public RTTI {
		RTTI_DECLARATIONS(EventPublisher, RTTI);

	public:

		// Constructor – Takes a list of subscribers and a boolean indicating whether to delete the event after publishing it (this second parameter could be optional, see note below).
		EventPublisher(std::vector<EventSubscriber*>& Subscribers, bool DeleteAfterPublish = false);

		// Destructor
		virtual ~EventPublisher();

		// Copy Semantics
		EventPublisher(const EventPublisher& other);

		// Copy Assignment
		virtual EventPublisher& operator=(const EventPublisher& rhs);
		
		// Move Semantics
		EventPublisher(EventPublisher&& other) noexcept;

		// Move Assignment
		virtual EventPublisher& operator=(EventPublisher&& rhs) noexcept;

		// TimeEnqueued – return the time when this event was enqueued.
		GameTime& TimeEnqueued();

		// Delay – return the amount of time after being enqueued that this event expires.
		GameTime& Delay();

		// IsExpired – takes the current time and returns true if the event has expired (time enqueued + delay).
		bool IsExpired(GameTime currentTime);

		// Deliver – Notify all subscribers of this event.
		void Deliver();

		// DeleteAfterPublishing – return whether this event should be deleted after it is published (could be optional, see note below).
		bool DeleteAfterPublishing();

	protected:
		std::vector<EventSubscriber*> m_subscribers;
		bool m_deleteOnPublish = false;
		GameClock clock;
		GameTime m_time_enqueued = clock.Current();
		GameTime m_Delay = clock.Current();
	};
}