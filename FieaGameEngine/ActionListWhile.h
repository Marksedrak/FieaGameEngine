#pragma once
#include "ActionList.h"

namespace Fiea::GameEngine {
	class ActionListWhile : public ActionList {
		RTTI_DECLARATIONS(ActionListWhile, ActionList);

	public:
		ActionListWhile(std::vector<RTTI::IdType>* Ids = nullptr) : ActionList(AppendId(Ids)) {};
		virtual ~ActionListWhile();
		ActionListWhile(const ActionListWhile& WhileList) = default;
		ActionListWhile(ActionListWhile&& WhileList) noexcept = default;
		ActionListWhile& operator=(const ActionListWhile& rhs) = default;
		ActionListWhile& operator=(ActionListWhile&& rhs) noexcept = default;
		[[nodiscard]] ActionListWhile* Clone() const override;

		void Update(GameTime time);
		void SetCondition(const string& conditionKey);

		static std::vector<Signature> Signatures();
	private:
		string condition = "\0";
		Datum* conditionDatum;

		std::vector<RTTI::IdType>* AppendId(std::vector<RTTI::IdType>* Ids);
		std::vector<RTTI::IdType>* ALWid;
	};
}