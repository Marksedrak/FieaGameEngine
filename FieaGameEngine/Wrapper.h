#pragma once
#include "RTTI.h"

namespace Fiea::GameEngine {
	class Wrapper : public RTTI
	{
		RTTI_DECLARATIONS(Wrapper, RTTI);

	public:

		virtual ~Wrapper() = default;

		void IncrementDepth();

		void DecrementDepth();

		int Depth();

	private:
		int m_Depth = 1;
	};
}

