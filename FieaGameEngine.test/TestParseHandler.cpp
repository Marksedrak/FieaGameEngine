#include "pch.h"
#include "TestParseHandler.h"

namespace Fiea::GameEngine::test
{
#pragma region TestParseHandler::TestWrapper
	RTTI_DEFINITIONS(TestParseHandler::TestWrapper);

	/**
	 * @brief RTTI override for Equals
	 * @param rhs 
	 * @return true if equal, false otherwise
	*/
	bool TestParseHandler::TestWrapper::Equals(const RTTI* rhs) const {
		if (rhs == nullptr) return false;

		const TestParseHandler::TestWrapper* other = rhs->As<TestParseHandler::TestWrapper>();
		return maxDepth == other->maxDepth;
	}

	/**
	 * @brief RTTI override for ToString
	 * @return string representation of this Wrapper
	*/
	std::string TestParseHandler::TestWrapper::ToString() const {
		return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
	}
#pragma endregion TestParseHandler::Wrapper

#pragma region TestParseHandler
	/**
	 * @brief Override for IParseHandler::Initialize
	*/
	void TestParseHandler::Initialize() {
		initializeCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Cleanup
	*/
	void TestParseHandler::Cleanup() {
		cleanupCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Start
	 * @param unused
	 * @param unused
	 * @param wrapper The wrapper to populate 
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::StartHandler(Wrapper& wrapper, const string& jsonKey, const Json::Value& value, bool IsArray) {
		TestWrapper* testWrapper = wrapper.As<TestWrapper>();
		if (testWrapper == nullptr) return false;

		if (testWrapper->Depth() >= (int)testWrapper->maxDepth) {
			testWrapper->maxDepth = testWrapper->Depth();
		}
		++startCount;
		return true;
	}

	/**
	 * @brief Override for IParseHandler::End
	 * @param unused
	 * @param wrapper The wrapper to populate
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::EndHandler(Wrapper& wrapper, const string& jsonKey) {
		Wrapper* testWrapper = wrapper.As<TestParseHandler::TestWrapper>();
		if (testWrapper == nullptr) return false;

		++endCount;
		return true;
	}
#pragma endregion TestParseHandler
}
