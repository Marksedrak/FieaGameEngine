#pragma once

#include "Foo.h"

namespace Fiea::GameEngine
{
	class FooChild : public Foo
	{
		RTTI_DECLARATIONS(FooChild, Foo);

	private:
		std::uint32_t value;
	};
}

