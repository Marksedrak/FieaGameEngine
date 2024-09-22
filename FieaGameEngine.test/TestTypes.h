#pragma once

#include "Datum.h"
#include "Scope.h"

using namespace Fiea::GameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<glm::vec4>(glm::vec4* v)
	{
		RETURN_WIDE_STRING(v);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4* v)
	{
		RETURN_WIDE_STRING(v);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& v)
	{
		RETURN_WIDE_STRING(glm::to_string(v).c_str());
	}

	template<>
	inline std::wstring ToString<glm::mat4>(glm::mat4* m)
	{
		RETURN_WIDE_STRING(m);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4* m)
	{
		RETURN_WIDE_STRING(m);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& m)
	{
		RETURN_WIDE_STRING(glm::to_string(m).c_str());
	}

	template<>
	inline std::wstring ToString<Datum>(Datum* d)
	{
		RETURN_WIDE_STRING(d->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum* d)
	{
		RETURN_WIDE_STRING(d);
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& d)
	{
		RETURN_WIDE_STRING(d.ToString().c_str());
	}


	template<>
	inline std::wstring ToString<RTTI>(RTTI* r)
	{
		RETURN_WIDE_STRING(r->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<RTTI>(const RTTI* r)
	{
		RETURN_WIDE_STRING(r->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<RTTI>(const RTTI& r)
	{
		RETURN_WIDE_STRING(r.ToString().c_str());
	}


	template<>
	inline std::wstring ToString<Scope>(Scope* s)
	{
		RETURN_WIDE_STRING(s->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope* s)
	{
		RETURN_WIDE_STRING(s->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope& s)
	{
		RETURN_WIDE_STRING(s.ToString().c_str());
	}

}
