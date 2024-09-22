#include "pch.h"
#include "EventApplyPoison.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Event<StatusEffect>);
	RTTI_DEFINITIONS(ApplyPoison);

	/**
	 * @brief ApplyPoison Event Constructor
	 * @param payload : StatusEffect struct
	 * @param delOnPub : bool deleteOnPublish
	*/
	ApplyPoison::ApplyPoison(StatusEffect& payload, bool delOnPub) : Event<StatusEffect>(payload, delOnPub) {};
}
