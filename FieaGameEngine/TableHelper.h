#pragma once
#include "pch.h"
#include "IParseHandler.h"
#include "Scope.h"

namespace Fiea::GameEngine {
	class TableHelper : public IParseHandler
	{
	public:
		class TableWrapper : public Wrapper {
			RTTI_DECLARATIONS(TableWrapper, Wrapper);
		public:
			TableWrapper(Scope& root) : rootScope(&root) {}

			void Append(int& value, const std::string& key, int idx = 0);
			void Append(float& value, const std::string& key, int idx = 0);
			void Append(std::string& value, const std::string& key, int idx = 0);
			void AppendObject(const std::string& key);
			void IncrementCurrentDepth();
			bool Verify(const std::string& key);

			void CheckDepth();

		private:
			void GoToParent();
			Scope* rootScope;
			int currentDepth = 1;
		};

		void Initialize() override;
		void Cleanup() override;

		bool StartHandler(Wrapper& wrapper, const string& jsonKey, const Json::Value& value, bool IsArray) override;
		bool EndHandler(Wrapper& wrapper, const string& jsonKey) override;
	};
}

