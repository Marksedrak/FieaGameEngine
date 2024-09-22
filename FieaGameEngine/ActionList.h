#pragma once
#include "Action.h"

namespace Fiea::GameEngine {
	class ActionList : public Action {
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		ActionList(std::vector<RTTI::IdType>* Ids = nullptr) : Action(AppendId(Ids)) {};

		virtual ~ActionList();
		ActionList(const ActionList& rhs) = default;
		ActionList(ActionList&& WhileList) noexcept = default;
		ActionList& operator=(const ActionList& rhs) = default;
		ActionList& operator=(ActionList&& rhs) noexcept = default;
		[[nodiscard]] ActionList* Clone() const override;

		bool operator==(ActionList* other);

		void Update(GameTime time);
		void AddAction(Action* action);

		static std::vector<Signature> Signatures();
	private:

		std::vector<RTTI::IdType>* AppendId(std::vector<RTTI::IdType>* Ids);
		std::vector<RTTI::IdType>* ALid;
	};
}