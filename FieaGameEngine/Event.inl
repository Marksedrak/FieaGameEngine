//#include "pch.h"
#include "Event.h"

namespace Fiea::GameEngine{
	
	// NOTE FOR FUTURE: ALL TEMPLATED FUNCTIONS SHOULD BE IN A .inl FILE SO THEY CAN BE COMPILED DURING COMPILETIME NOT RUNTIME. TEMPLATES NEED TO BE DEFINED AND READABLE DURING COMPILETIME
	// CPP files create Objects that the linker creates to hold definitions, but templated methods can have any number of definitions so they can't be included in an object
	template <typename EventType>
	RTTI_DEFINITIONS(Event<EventType>);

	/** Constructor
	 * @brief Constructs a new event
	 * @tparam T
	 * @param Message : Payload
	 * @param onPublishDelete : If event will be destroyed after being published
	*/
	template <typename EventType>
	Event<EventType>::Event(EventType Message, bool onPublishDelete) : EventPublisher(m_eventSubscribers, onPublishDelete) {
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
	Event<EventType>::Event(const Event<EventType>& other) : EventPublisher(other), m_Message(other.m_Message) {};


	/** Copy Assignment
	 * @brief Copies using the = operator
	 * @tparam T
	 * @param rhs : Event to copy from
	 * @return Copied Event
	*/
	template <typename EventType>
	Event<EventType>& Event<EventType>::operator=(const Event<EventType>& rhs) {
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
	Event<EventType>::Event(Event<EventType>&& other) : EventPublisher(std::move(other)), m_Message(std::move(other.m_Message)) {}


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