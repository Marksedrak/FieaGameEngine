#include "pch.h"
#include "EventApplyPoison.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ApplyPoison);
	
	ApplyPoison::ApplyPoison(StatusEffect& payload, bool delOnPub) : Event<StatusEffect>(payload, delOnPub) {};
}
