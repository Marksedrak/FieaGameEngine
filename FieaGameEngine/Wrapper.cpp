#include "pch.h"
#include "Wrapper.h"

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(Wrapper);
	
	void Wrapper::IncrementDepth() {
		++m_Depth;
	}

	void Wrapper::DecrementDepth() {
		--m_Depth;
	}

	int Wrapper::Depth() {
		return m_Depth;
	}

}
