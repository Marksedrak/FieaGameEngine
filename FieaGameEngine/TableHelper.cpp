#pragma once
#include "pch.h"
#include "TableHelper.h"
#include <regex>
#include "Factory.h"
#include "GameObject.h"

namespace Fiea::GameEngine {

#pragma region TableHelper::TableWrapper
	// TableWrapper
	RTTI_DEFINITIONS(TableHelper::TableWrapper);
	
	/**
	 * @brief Appends an Int to the underlying Scope
	 * @param value: int to append
	 * @param key: Key to append under
	*/
	void TableHelper::TableWrapper::Append(int& value, const string& key, int idx) {
		//Datum& newDatum = rootScope->Append(key);
		// Find out which Class it is

		Attributed* att = rootScope->As<Attributed>();
		if (att != nullptr) {
			if (att->IsPrescribedAttribute(key)) {
				Datum* newDatum = rootScope->Find(key);
				if (newDatum->CheckType(Datum::DatumType::Float)) {
					float floatValue = (float)value;
					newDatum->Set(idx, floatValue);
				}
				else{
					newDatum->Set(idx, value);
				}
			}
			else {
				Datum& newDatum = rootScope->Append(key);
				newDatum.Push(value);
			}
		}
		else {
			Datum& newDatum = rootScope->Append(key);
			newDatum.Push(value);
		}
	}

	/**
	 * @brief Appends a float to the current wrapper object
	 * @param value : value to append
	 * @param key : Key to append to
	*/
	void TableHelper::TableWrapper::Append(float& value, const std::string& key, int idx)
	{
		Attributed* att = rootScope->As<Attributed>();
		if (att != nullptr) {
			if (att->IsPrescribedAttribute(key)) {
				Datum* newDatum = rootScope->Find(key);
				newDatum->Set(idx, value);
			}
			else {
				Datum& newDatum = rootScope->Append(key);
				newDatum.Push(value);
			}
		}
		else {
			Datum& newDatum = rootScope->Append(key);
			newDatum.Push(value);
		}
	}

	/**
	 * @brief Appends either a String, Vector, or Matrix
	 * @param value : Value to be appended
	 * @param key : Key to append to
	 * @param idx : index of the Appended value, defaults to 0
	*/
	void TableHelper::TableWrapper::Append(std::string& value, const std::string& key, int idx)
	{
		Datum& newDatum = rootScope->Append(key);
		newDatum.SetFromString(idx, value);
	}

	/**
	 * @brief Adds a scope to the current Object(Scope) and sets the scope pointer to the child
	 * Sets the Wrapper for Depth-first population
	 * @param key : key to Append the scope under
	*/
	void TableHelper::TableWrapper::AppendObject(const std::string& key)
	{
		// Check if Class specified
		std::regex classPattern("\\{([^{}]*)\\}");
		std::regex namePattern("\\{[^{}]*\\}(\\w+)");
		std::smatch match;
		if (std::regex_search(key, match, classPattern)) {
			std::string className = match[1];
			if (std::regex_search(key, match, namePattern)) {
				std::string objName = match[1];
				Scope& child = rootScope->AppendScope(objName, FactoryManager<Scope>::Create(className));
				rootScope = &child;
			}
		}
		else {
			Scope& child = rootScope->AppendScope(key);
			rootScope = &child;
		}
	}

	/**
	 * @brief Increments current depth
	*/
	void TableHelper::TableWrapper::IncrementCurrentDepth()
	{
		++currentDepth;
	}

	/**
	 * @brief Verifies if the key exists in the underlying scope
	 * @param key : key to check for
	 * @return True if exists, false otherwise
	*/
	bool TableHelper::TableWrapper::Verify(const std::string& key)
	{
		if (rootScope->Find(key) == nullptr) return false;
		return true;
	}

	/**
	 * @brief Checks if the Wrapper's Depth is the same as current depth, if not then Goes to this Scope's Parent
	*/
	void TableHelper::TableWrapper::CheckDepth()
	{
		while (Depth() < currentDepth) {
			GoToParent();
			--currentDepth;
		}
	}

	/**
	 * @brief sets current scope (rootScope) to the Parent of the current Scope if one exists
	*/
	void TableHelper::TableWrapper::GoToParent()
	{
		if (rootScope->GetParent() != nullptr) {
			rootScope = rootScope->GetParent();
		}
	}

#pragma endregion TableHelper::TableWrapper

#pragma region TableHelper
	// TableHelper

	void TableHelper::Initialize()
	{
		// Doesn't do anything for now
	}

	void TableHelper::Cleanup()
	{
		// Also doesn't do anything for now
	}

	/**
	 * @brief Attempts to handle the json value that is passed to it
	 * @param wrapper : wrapper
	 * @param jsonKey : Member name used as key for th string/Datum pair created
	 * @param value : value associated with the jsonKey
	 * @param IsArray : if value passed is am array
	 * @return True if handled successfully, false otherwise
	*/
	bool TableHelper::StartHandler(Wrapper& wrapper, const string& jsonKey, const Json::Value& value, bool IsArray)
	{
		TableWrapper* Twrapper = wrapper.As<TableWrapper>();
		if (Twrapper == nullptr) return false;

		//Check if depth is correct
		Twrapper->CheckDepth();

		if (IsArray) {
			if (value[0].isInt())
			{
				for (int idx = 0; idx < (int)value.size(); ++idx) {
					int val = value[idx].asInt();
					Twrapper->Append(val, jsonKey, idx);
				}
					return true;
			}
			else if (value[0].isDouble()) {
				for (int idx = 0; idx < (int)value.size(); ++idx) {
					float val = value[idx].asFloat();
					Twrapper->Append(val, jsonKey, idx);
				}
				return true;
			}
			else if (value[0].isString()) {
				// Handle Strings, Vec4s, Mat4s
				for (int idx = 0; idx < (int)value.size(); ++idx) {
					std::string val = value[idx].asString();
					Twrapper->Append(val, jsonKey, idx);
				}
				return true;
			}
			else if (value[0].isObject()) {
				Twrapper->AppendObject(jsonKey);
				Twrapper->IncrementCurrentDepth();
				return true;
			}
		}
		else if (value.isObject()) {
			Twrapper->AppendObject(jsonKey);
			Twrapper->IncrementCurrentDepth();
			return true;
		}
		else {
			if (value.isInt())
			{
				int val = value.asInt();
				// Check if reading a prescribed attribute
				Twrapper->Append(val, jsonKey);
				return true;
			}
			else if (value.isDouble()) {
				float val = value.asFloat();
				Twrapper->Append(val, jsonKey);
				return true;
			}
			else if (value.isString()) {
				// Handle Strings, Vec4s, Mat4s
				std::string val = value.asString();
				Twrapper->Append(val, jsonKey);
				return true;
			}
		}
		return false;
	}

	/**
	 * @brief Check if the handling was indeed succesful
	 * @param wrapper : wrapper to check
	 * @param jsonKey : key to verify
	 * @return True if correctly handled, False otherwise
	*/
	bool TableHelper::EndHandler(Wrapper& wrapper, const string& jsonKey)
	{

		TableWrapper* TWrapper = wrapper.As<TableWrapper>();
		if (TWrapper == nullptr) return false;
		TWrapper->CheckDepth();
		return TWrapper->Verify(jsonKey);
	}

#pragma endregion TableHelper

}