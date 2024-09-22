#pragma once
#include "Event.h"

namespace Fiea::GameEngine {

	struct StatusEffect {
		int Damage = 0;
		std::string StatusName = "";
	};

	class ApplyPoison final : public Event<StatusEffect> {
		RTTI_DECLARATIONS(ApplyPoison, Event<StatusEffect>);

	public:
		ApplyPoison(StatusEffect& payload, bool delOnPub);
		
	};
}