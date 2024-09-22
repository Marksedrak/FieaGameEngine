#pragma once

#include "RTTI.h"
#include <cstdint>

namespace Fiea::GameEngine
{
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI);

	public:
		explicit Foo(std::int32_t data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		virtual ~Foo();

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

		std::int32_t Data() const;
		void SetData(std::int32_t data);

		std::string ToString() const override;
		bool Equals(const RTTI* rhs) const override;

	private:
		std::int32_t* _data;
	};
}