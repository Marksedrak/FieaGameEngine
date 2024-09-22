#pragma once
#include "json/json.h"
#include "Wrapper.h"

using string = std::string;

namespace Fiea::GameEngine {
	class IParseHandler
	{
	public:
		virtual void Initialize() = 0;

		virtual void Cleanup() = 0;

		virtual bool StartHandler(Wrapper& wrapper, const string& jsonKey, const Json::Value& value, bool IsArray) = 0;

		virtual bool EndHandler(Wrapper& wrapper, const string& jsonKey) = 0;
	};
}

