#include "pch.h"
#include "ParseCoordinator.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <istream>

namespace Fiea::GameEngine {

	/**
	 * @brief Constructor
	 * @param wrapper : Wrapper of data to be populated
	*/
	ParseCoordinator::ParseCoordinator(Wrapper& wrapper) {
		m_Wrapper = &wrapper;
	}

	/**
	 * @brief Adds Handler to vector of handlers
	 * @param Handler : handler to add
	*/
	void ParseCoordinator::AddHandler(IParseHandler* Handler) {
		// std::make_shared tries to construct an IParseHandler with Handler as a parameter and returning
		// a shared pointer of that (Not what we want)
		std::shared_ptr<IParseHandler> sharedPtr(Handler);
		m_Handlers.push_back(std::move(sharedPtr));
	}

	/**
	 * @brief Removes handler from list of handlers
	 * @param Handler : handler to remove
	*/
	void ParseCoordinator::RemoveHandler(IParseHandler* Handler) {
		// Simply erases handler from vector since it will be destructed on it's own
		for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it) {
			if (it->get() == Handler) {
				it = m_Handlers.erase(it);
				break;
			}
		}
	}

	/**
	 * @brief Deserializes json string into Json::Value and passes it to ParseMembers
	 * @param json as a string
	 * @return True if successful, otherwise false
	*/
	bool ParseCoordinator::DeserializeObject(const string& json)
	{
		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();

		Json::Value root;
		std::string errors;
		bool readSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
		delete reader;

		if (readSuccessful) {
			return ParseMembers(root);
		}
		return false;
	}

	bool ParseCoordinator::DeserializeObjectFromFile(const string& filename)
	{
		std::ifstream file(filename);
		
		// Check if file was correctly accessed
		if (!file.is_open()) {
			return false;
		}

		std::string jsonStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		// Closing file
		file.close();

		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();

		Json::Value root;
		std::string errors;
		bool readSuccessful = reader->parse(jsonStr.c_str(), jsonStr.c_str() + jsonStr.size(), &root, &errors);
		delete reader;

		if (readSuccessful) {
			return ParseMembers(root);
		}
		return false;
	}

	bool ParseCoordinator::DeserializeObject(std::istream& jsonStream)
	{

		std::stringstream buffer;
		buffer << jsonStream.rdbuf();

		std::string jsonString = buffer.str();


		// None-deprecated parsing method of jsoncpp
		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();

		Json::Value root;
		std::string errors;
		bool readSuccessful = reader->parse(jsonString.c_str(), jsonString.c_str() + jsonString.size(), &root, &errors);

		// Clean up
		delete reader;

		if (readSuccessful) {
			return ParseMembers(root);
		}
		return false;
	}

	/**
	 * @brief Retrieves the Wrapper variable
	 * @return m_Wrapper
	*/
	Wrapper* ParseCoordinator::GetWrapper()
	{
		return m_Wrapper;
	}

	const Wrapper* ParseCoordinator::GetWrapper() const
	{
		return m_Wrapper;
	}

	/**
	 * @brief Calls Parse on each member of the root Json::Value
	 * @param members : root Json::Value
	*/
	bool ParseCoordinator::ParseMembers(const Json::Value& members)
	{
		for (const auto& keyValue : members.getMemberNames()) {
			const string& key = keyValue;
			const Json::Value& value = members[key];
			bool isArray = value.isArray();
			if (!Parse(key, value, isArray)) {
				return false;
			}
		}
		return true;
	}

	/**
	 * @brief Attemps to Parse key/value pair using the different handlers added
	 * @param key : Key of json value
	 * @param value : json value
	 * @param isArray : bool if the value is an array
	*/
	bool ParseCoordinator::Parse(const string& key, const Json::Value& value, bool isArray)
	{
		bool parseingSuccessful = false;
		for (const auto& handler : m_Handlers) {
			handler->Initialize();
			bool success = handler->StartHandler(*m_Wrapper, key, value, isArray);
			if (success) {
				if (value.isObject()) {
					m_Wrapper->IncrementDepth();
					ParseMembers(value);
					m_Wrapper->DecrementDepth();
				}
				else if (value.isArray() && value[0].isObject()) {
					for (int idx = 0; idx < (int)value.size(); ++idx) {
						m_Wrapper->IncrementDepth();
						ParseMembers(value[idx]);
						m_Wrapper->DecrementDepth();
					}
				}
				handler->EndHandler(*m_Wrapper, key);
				parseingSuccessful = success;
			}
			handler->Cleanup();
		}
		return parseingSuccessful;
	}


}
