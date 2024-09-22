#pragma once
#include "pch.h"
#include "TestIntHandler.h"

namespace Fiea::GameEngine::test {
	
#pragma region TestIntHandler::TestIntWrapper
	RTTI_DEFINITIONS(TestIntHandler::TestIntWrapper);

	TestIntHandler::TestIntWrapper::~TestIntWrapper()
	{
		delete[] m_wrappedInt;
	}

	bool TestIntHandler::TestIntWrapper::Equals(const RTTI* rhs) const
	{
		if (rhs == this) return false;

		const TestIntHandler::TestIntWrapper* other = rhs->As<TestIntHandler::TestIntWrapper>();
		return other->GetInt() == GetInt();
	}

	/**
	 * @brief Get int* wrapped in
	 * @return m_wrappedInt
	*/
	int TestIntHandler::TestIntWrapper::GetInt(int idx) {
		return m_wrappedInt[idx];
	}

	const int TestIntHandler::TestIntWrapper::GetInt(int idx) const {
		return m_wrappedInt[idx];
	}

	void TestIntHandler::TestIntWrapper::SetArray(int arraySize)
	{
		delete[] m_wrappedInt;
		m_wrappedInt = new int[arraySize];
		m_size = arraySize;
	}

	void TestIntHandler::TestIntWrapper::Set(int* value, int index)
	{
		if (index >= m_size || index < 0) {
			throw std::out_of_range("index out of range");
		}
		else {
			m_wrappedInt[index] = *value;
		}
	}

	/**
	 * @brief Get the size of the int, 1 if a single integer, otherwise an array;
	 * @return m_size;
	*/
	int& TestIntHandler::TestIntWrapper::Size()
	{
		return m_size;
	}

	const int& TestIntHandler::TestIntWrapper::Size() const
	{
		return m_size;
	}
	
#pragma endregion TestParseHandler::Wrapper

#pragma region TestIntHandler

	void TestIntHandler::Initialize() {

	}

	void TestIntHandler::Cleanup() {

	}

	/**
	 * @brief Attempts to handle the wrapper
	 * @param wrapper : wrapper to try handle
	 * @param jsonKey : key to extract from Json data
	 * @param value : value corresponding to the key
	 * @param IsArray : bool to determine if the value is an array
	 * @return True if successfully handled wrapper, otherwise false
	*/
	bool TestIntHandler::StartHandler(Wrapper& wrapper, const string& jsonKey, const Json::Value& value, bool IsArray) {
		TestIntWrapper* testWrapper = wrapper.As<TestIntWrapper>();
		if (testWrapper == nullptr) return false;
		int* newInt = new int();

		if (IsArray) {
			// Check if value is in fact an array
			if (value.isArray() && value[0].isInt()) {
				testWrapper->SetArray(value.size());
				for (int i = 0; i < (int)value.size(); ++i) {
					*newInt = value[i].asInt();
					testWrapper->Set(newInt, i);
				}
				delete newInt;
				return true;
			}
			else {
				delete newInt;
				return false;
			}
		}
		else {
			if (value.isInt()) {
				*newInt = value.asInt();
				testWrapper->SetArray(1);
				testWrapper->Set(newInt);
				delete newInt;
				return true;
			}
			else {
				delete newInt;
				return false;
			}
		}
	}

	/**
	 * @brief Ends Handler, verifying handling was completed correctly
	 * @param wrapper : wrapper to try handle
	 * @param jsonKey : key to extract from Json data
	 * @return True if correclty handled wrapper, otherwise false
	*/
	bool TestIntHandler::EndHandler(Wrapper& wrapper, const string& jsonKey) {
		return false;
	}

#pragma endregion TestIntHandler
}
