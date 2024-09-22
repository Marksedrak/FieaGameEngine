#include "pch.h"
#include "Event.h"

namespace Fiea::GameEngine {
	template <typename EventType>
	RTTI_DEFINITIONS(Event<EventType>);


	template <typename EventType>
	std::vector<EventSubscriber*> Event<EventType>::m_eventSubscribers;

	/** Constructor
	 * @brief Constructs a new event
	 * @tparam T 
	 * @param Message : Payload 
	 * @param onPublishDelete : If event will be destroyed after being published
	*/
	template <typename EventType>
	inline Event<EventType>::Event(EventType Message, bool onPublishDelete) : EventPublisher(m_eventSubscribers, onPublishDelete) {
		// Create IMessage interaface and compare T to it to makes sure only Message/Payload type
		// classes are used with event
		// StatusEffect needs to derived from IMessage

		m_Message = Message;
		m_deleteOnPublish = onPublishDelete;
	}


	/** Copy Constructor
	 * @brief Creates an event by copying an existing event
	 * @tparam T 
	 * @param other : original Event to copy from
	*/
	template <typename EventType>
	inline Event<EventType>::Event(const Event& other) : EventPublisher(other), m_Message(other.m_Message) {};


	/** Copy Assignment
	 * @brief Copies using the = operator
	 * @tparam T 
	 * @param rhs : Event to copy from
	 * @return Copied Event
	*/
	template <typename EventType>
	inline Event<EventType>& Event<EventType>::operator=(const Event& rhs){
		if (this != &rhs) {
			EventPublisher::operator=(rhs);
			m_Message = rhs.m_Message;
		}
		return *this;
	}

	/** Move Constructor
	 * @brief Constructs a new Event by moving an existing one into it
	 * @tparam T 
	 * @param other : Event to move inot newly created Event
	*/
	template<typename EventType>
	inline Event<EventType>::Event(Event&& other) : EventPublisher(std::move(other)), m_Message(std::move(other.m_Message)) {}


	/** Move Assignment
	 * @brief Moves existing Event into another Event
	 * @tparam T 
	 * @param rhs : Event to move
	 * @return Event with moved Event's attributes
	*/
	template<typename EventType>
	inline Event<EventType>& Event<EventType>::operator=(Event&& rhs)
	{
		if (this != &rhs) {
			EventPublisher::operator=(std::move(rhs));
			m_Message = std::move(rhs.m_Message);
		}
		return *this;
	}


	/** Message
	 * @brief Gets the Message/Payload
	 * @tparam T 
	 * @return the Event's Message/Payload
	*/
	template<typename EventType>
	inline EventType& Event<EventType>::Message()
	{
		return m_Message;
	};

}