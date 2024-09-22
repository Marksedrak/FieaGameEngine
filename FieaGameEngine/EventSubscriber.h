#pragma once
#include "RTTI.h"

namespace Fiea::GameEngine {
	class EventPublisher;
	class EventSubscriber : public RTTI {
		RTTI_DECLARATIONS(EventSubscriber, RTTI);

	public:
		// Notify – pure virtual method that accepts the address of an EventPublisher (the actual argument will be the event itself). 
		// Your handlers will use the RTTI interface to verify actual the event type.
		virtual void Notify(EventPublisher* publisher) = 0;

	};
}
