#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"
#include "GameClock.h"

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(EventPublisher);

	/** Constructor
	 * @brief Constructs an EventPublisher
	 * @param Subscribers: vector of Subscribers
	 * @param DeleteAfterPublish (optional): determines if the event would be destroy after publishing
	*/
	EventPublisher::EventPublisher(std::vector<EventSubscriber*>& Subscribers, bool DeleteAfterPublish) : m_subscribers(Subscribers), m_deleteOnPublish(DeleteAfterPublish) {};


	/** Destructor
	 * @brief Destructs EventPublisher
	*/
	EventPublisher::~EventPublisher() {
		m_subscribers.clear();
	}

	/** Copy Constructor
	 * @brief Constructs a copy a EventPublisher
	 * @param other: EventPublisher to copy from
	*/
	EventPublisher::EventPublisher(const EventPublisher& other) : m_subscribers(other.m_subscribers), m_deleteOnPublish(other.m_deleteOnPublish) {};

	/** Copy Assignment
	 * @brief Copies an EventPublisher into another
	 * @param rhs: EventPublisher to copy from
	 * @return Copy of right-hand-side EventPublisher
	*/
	EventPublisher& EventPublisher::operator=(const EventPublisher& rhs) {
		if (this != &rhs) {
			m_subscribers = rhs.m_subscribers;
			m_deleteOnPublish = rhs.m_deleteOnPublish;
		}
		return *this;
	}


	/** Move Constructor
	 * @brief Creates an EventPublisher by Moving another one into it
	 * @param rhs: EventPublisher to move into the newly constructed EventPublisher
	*/
	EventPublisher::EventPublisher(EventPublisher&& other) noexcept : m_subscribers(std::move(other.m_subscribers)), m_deleteOnPublish(other.m_deleteOnPublish) {};


	/** Move Assignment
	 * @brief Moves rhs EventPublisher into lhs
	 * @param rhs: EventPublisher to move
	 * @return EventPublisher with moved rhs
	*/
	EventPublisher& EventPublisher::operator=(EventPublisher&& rhs) noexcept {
		if (this != &rhs) {
			m_subscribers = std::move(rhs.m_subscribers);
			m_deleteOnPublish = rhs.m_deleteOnPublish;
		}
		return *this;
	}


	/** TimeEnqueued
	 * @brief Returns the time when this event was Enqueued
	 * @return GameTime
	*/
	GameTime& EventPublisher::TimeEnqueued()
	{
		return m_time_enqueued;
	}


	/** Delay
	 * @brief Return the amount of time after being enqueued that this event expires
	 * @return time after being enqueued that this event expires
	*/
	GameTime& EventPublisher::Delay()
	{
		return m_Delay;
	}


	/** IsExpired
	 * @brief Takes current time returns true if the event has expired (CurrentTime > (TimeEnqueued + Delay))
	 * @param currentTime 
	 * @return True if Expired, False otherwise
	*/
	bool EventPublisher::IsExpired(GameTime currentTime) {
		return (currentTime.Get() > (TimeEnqueued().Get() + Delay().Get()));
	}


	/** Deliver
	 * @brief Notify all subscribers of this event
	*/
	void EventPublisher::Deliver()
	{
		for (EventSubscriber* subscriber : m_subscribers) {
			subscriber->Notify(this);
		}
	}

	/** DeleteAfterPublishing
	 * @brief Returns wether this event is set to delete after publishing
	 * @return True if set to delete, otherwise false
	*/
	bool EventPublisher::DeleteAfterPublishing()
	{
		return m_deleteOnPublish;
	}


}
