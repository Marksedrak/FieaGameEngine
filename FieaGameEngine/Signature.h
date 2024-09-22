#pragma once

#include "Datum.h"
using string = std::string;

namespace Fiea::GameEngine {
	class Signature
	{
	public: 
		string Name;
		Datum::DatumType Type;
		uint32_t size;
		size_t Offset;
	};
}

