#pragma once
#include "Action.h"

namespace Fiea::GameEngine{
	class ActionIncrement : public Action {
		RTTI_DECLARATIONS(ActionIncrement, Action);
	
	public:
		ActionIncrement(std::vector<RTTI::IdType>* Ids = nullptr) : Action(AppendId(Ids)) {};

		ActionIncrement(const ActionIncrement& rhs) = default;
		ActionIncrement(ActionIncrement&& rhs) noexcept = default;
		ActionIncrement& operator=(const ActionIncrement& rhs) = default;
		ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;
		virtual ~ActionIncrement();
		[[nodiscard]] ActionIncrement* Clone() const override;

		bool operator==(ActionIncrement* other);

		void Update(GameTime time) override;
		void SetDatumKey(const string& key);
		void SetValue(float value);

		// Maybe implement dot syntax

		static std::vector<Signature> Signatures();

	private:
		string DatumKey = "";
		float Value = 0.0f;
		Datum* IncrementDatum = nullptr;
		bool isArray = false;
		int idx = -1;

		std::vector<RTTI::IdType>* AppendId(std::vector<RTTI::IdType>* Ids);
		std::vector<RTTI::IdType>* AIid; // For some reason, setting this to nullptr causes memory leaks
	};
}