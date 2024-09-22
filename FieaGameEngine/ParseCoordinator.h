#pragma once
#include "Wrapper.h"
#include "IParseHandler.h"
#include <vector>

using string = std::string;

namespace Fiea::GameEngine {
	class ParseCoordinator
	{
	public:
		explicit ParseCoordinator(Wrapper& wrapper);

		~ParseCoordinator() = default;

		// Deleted Copy, Move, and Assignment Semantics
		ParseCoordinator(ParseCoordinator& other) = delete;
		ParseCoordinator(ParseCoordinator&& m_other) = delete;
		ParseCoordinator operator=(ParseCoordinator& rhs) = delete;
		ParseCoordinator operator=(ParseCoordinator&& m_rhs) = delete;

		void AddHandler(IParseHandler* Handler);
		void RemoveHandler(IParseHandler* Handler);

		bool DeserializeObject(const string& json);
		bool DeserializeObjectFromFile(const string& filename);
		bool DeserializeObject(std::istream& jsonStream);

		Wrapper* GetWrapper();
		const Wrapper* GetWrapper() const;

	private:

		bool ParseMembers(const Json::Value& members);
		bool Parse(const string& key,const Json::Value& value, bool isArray);

		Wrapper* m_Wrapper;
		std::vector<std::shared_ptr<IParseHandler>> m_Handlers;
	};
}

