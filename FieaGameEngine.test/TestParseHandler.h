/// <summary>
/// The declaration of the test parse handler and its wrapper.
/// </summary>

#pragma once
#include "IParseHandler.h"

namespace Fiea::GameEngine::test
{
	class TestParseHandler final : public Fiea::GameEngine::IParseHandler {
	public:
		class TestWrapper : public Fiea::GameEngine::Wrapper {
			RTTI_DECLARATIONS(TestWrapper, Wrapper);

		public:
			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;
		};

		void Initialize() override;
		void Cleanup() override;
		bool StartHandler(Wrapper& wrapper, const string& jsonKey, const Json::Value& value, bool IsArray) override;
		bool EndHandler(Wrapper& wrapper, const string& jsonKey) override;

		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}

