#include "pch.h"
#include "ActionIncrement.h"
#include <regex>

using namespace std::string_literals;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(ActionIncrement);

	/** Destructor
	 * @brief Deconstruct the only Owned member in case it was newed
	*/
	ActionIncrement::~ActionIncrement()
	{
		if (AIid != nullptr) {
			if (!AIid->empty()) {
				delete AIid;
			}
		}
	}

	/**
	 * @brief Clones ActionIncrement
	 * @return pointer to new ActionIncrement with this' attributes
	*/
	ActionIncrement* ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	/** Compares two ActionIncrements based on Value and DatumKey
	 * @brief 
	 * @param other 
	 * @return 
	*/
	bool ActionIncrement::operator==(ActionIncrement* other)
	{
		return (DatumKey == other->DatumKey && Value == other->Value);
	}

	/**Update
	 * @brief Adds value to the Datum with DatumKey
	 * @param time
	*/
	void ActionIncrement::Update(GameTime time)
	{
		SetDatumKey(DatumKey);
		assert(IncrementDatum != nullptr);

		if (IncrementDatum->CheckType(Datum::DatumType::Int)) {
			if (isArray) {
				int result = IncrementDatum->Get<int>(idx) + (int)Value;
				IncrementDatum->Set(idx, result);
			}
			int result = IncrementDatum->Get<int>() + (int)Value;
			IncrementDatum->Set(0, result);
		}
		else if (IncrementDatum->CheckType(Datum::DatumType::Float)) {
			if (isArray) {
				float result = IncrementDatum->Get<float>(idx) + Value;
				IncrementDatum->Set(idx, result);
			}
			float result = IncrementDatum->Get<float>() + Value;
			IncrementDatum->Set(0, result);
		}
	}

	/** SetDatumKey
	 * @brief Set DatumKey and Finds and sets IncrementDatum
	 * @param key : key to set DatumKey to
	*/
	void ActionIncrement::SetDatumKey(const string& key)
	{
		// Make sure Game Object parent is set before trying to set Increment Datum
		if (GOparent == nullptr) {
			throw std::runtime_error("Game Object parent was not set, please use SetParent to set the parent Game object");
		}

		isArray = false;

		// Checks for dot notation or bracket notation
		size_t dotLocation = key.find_first_of(".");
		size_t openbracketLocation = key.find_first_of("[");
		size_t closebracketLocation = key.find_first_of("]");

		bool dotExsists = (dotLocation != string::npos);
		bool bracketsCompleted = (openbracketLocation != string::npos && closebracketLocation != string::npos);

		DatumKey = key;
		// used Exclusive OR to prevent usage of both bracket and dot notation
 		if (dotExsists ^ bracketsCompleted) {
			if (dotExsists) {
				// Get the Table-type Datum
				Datum* ScopeArray = GOparent->Find("Children");
				// Iterating through the Datum to find a matching Datum
				for (int idx = 0; idx < (int)ScopeArray->Size(); ++idx) {
					if (ScopeArray->GetScope(idx)->Find(key.substr(0, dotLocation)) != nullptr) {
						// If a Datum is found assign a pointer of it to IncrementDatum
						IncrementDatum = ScopeArray->GetScope(idx)->Find(key.substr(0, dotLocation))->GetScope()->Find(key.substr(dotLocation + 1, key.size()));
					}
				}
			}
			else {
				std::regex rgx("\\[(\\d+)\\]");
				std::smatch match;

				if (std::regex_search(key, match, rgx)) {
					idx = std::stoi(match[1].str());
					// This should only work for arrays of either Floats, Integers, or Strings (includes Vectors and Matrices)
					IncrementDatum = GOparent->Find(key.substr(0, openbracketLocation));
					isArray = true;
				}
				else {
					throw std::invalid_argument("Invalid input in brackets");
				}
			}
		}
		else {
			DatumKey = key;
			IncrementDatum = GOparent->Find(DatumKey);

			// Checks if a valid datum was found
			if (IncrementDatum == nullptr) {
				throw std::runtime_error("Invalid key or key does not exist in parent Game object");
			}
		}
	}

	/** SetValue
	 * @brief Set Value
	 * @param value: value to set for Value
	*/
	void ActionIncrement::SetValue(float value)
	{
		Value = value;
	}

	std::vector<RTTI::IdType>* ActionIncrement::AppendId(std::vector<RTTI::IdType>* Ids)
	{
		if (Ids == nullptr) {
			AIid = NEW std::vector<RTTI::IdType>();
			AIid->push_back(TypeIdClass());
			return AIid;
		}
		Ids->push_back(TypeIdClass());
		return Ids;
	}

	/**
	 * @brief Returns class' Signatures
	 * @return vector of Signatures
	*/
	std::vector<Signature> ActionIncrement::Signatures()
	{
		return std::vector<Signature> {
			{ "DatumKey"s, Datum::DatumType::String, 1, offsetof(ActionIncrement, DatumKey) },
			{ "Value"s, Datum::DatumType::Float, 1, offsetof(ActionIncrement, Value) }
		};
	}
}
