#pragma once
#include "IParseHandler.h"

namespace Fiea::GameEngine::test {
	class TestIntHandler final : public Fiea::GameEngine::IParseHandler
	{
	public:
		//Embedded TestIntWrapper
		class TestIntWrapper : public Fiea::GameEngine::Wrapper {
			RTTI_DECLARATIONS(TestIntHandler::TestIntWrapper, Wrapper);

		public:
			~TestIntWrapper();

			bool Equals(const RTTI* rhs) const;

			int GetInt(int idx = 0);
			const int GetInt(int idx = 0) const;

			void SetArray(int arraySize);
			void Set(int* value, int index = 0);

			int& Size();
			const int& Size() const;

		private:

			int* m_wrappedInt = nullptr;
			int m_size = 0;
		};

		// TestIntHandler START
		void Initialize() override;
		void Cleanup() override;

		bool StartHandler(Wrapper& wrapper, const string& jsonKey, const Json::Value& value, bool IsArray) override;
		bool EndHandler(Wrapper& wrapper, const string& jsonKey) override;
	};
}

