#include "pch.h"
#include "ActionListWhile.h"
#include "ActionIncrement.h"

using namespace std::string_literals;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(ActionListWhile);

	/** Destructor
	 * @brief Destructor taking care of owned data
	*/
	ActionListWhile::~ActionListWhile()
	{
		if (ALWid != nullptr) {
			if (!ALWid->empty()) {
				delete ALWid;
			}
		}
	}

	ActionListWhile* ActionListWhile::Clone() const
	{
		return NEW ActionListWhile(*this);
	}

	/** Update
	 * @brief Executes Preamble first the start a while loop to update each Action in the list till the condition is met
	 * @param time 
	*/
	void ActionListWhile::Update(GameTime time)
	{
		Datum* IncrementDatum = Find("Increment");
		if (IncrementDatum->Empty()) {				// If the Increment Datum is empty, adds an Action increment that defaultly decrements by 1
			// Add ActionIncrement as default
			//ActionList* AL = new ActionList();
			ActionIncrement* increment = new ActionIncrement();
			increment->SetParent(GOparent);			// Sets increment's parent to ActionListWhile's GameObject Parent
			increment->SetDatumKey(condition);	// Sets the Datum key to alter
			increment->SetValue(-1);				// Set value to increment by to -1 so it decrementes by 1
			Adopt(*increment, "Increment");		// Pushes the ActionIncrement into the Increment Datum
		}
		// Call update on Preamble actions
		Datum* Preamble = Find("Preamble");
		if (Preamble->Size() > 0) {
			for (int idx = 0; idx < (int)Preamble->GetScope()->GetSize(); ++idx) {
				if (!(*Preamble->GetScope())[idx].Empty()) {
					Action* preambleAction = (*Preamble->GetScope())[idx].GetScope()->As<Action>();
					if (preambleAction != nullptr) {
						preambleAction->SetParent(GOparent);
						preambleAction->Update(time);
					}
					else {
						throw std::runtime_error("Preamble action invalid at index " + idx);
					}
				}
			}
		}
		// For easier access get the increment Action
		Action* incrementAction = Find("Increment")->GetScope()->As<Action>();

		if (conditionDatum == nullptr) {
			// Set conditionDatum based on the condition 
			conditionDatum = GOparent->Find(condition);
			if (conditionDatum == nullptr) {
				throw std::invalid_argument("Invalid condition datum");
			}
		}
		// Execute while loop for ActionListWhile
		while (conditionDatum->Get<int>()) {				// Will run as long as condition is non-zero
			Datum* Actions = Find("Actions");
			if (Actions->Size() > 0) {
				for (int actionIdx = 0; actionIdx < (int)Actions->GetScope()->GetSize(); ++actionIdx) {
					Action* loopAction = (*Actions->GetScope())[actionIdx].GetScope()->As<Action>();
					if (loopAction != nullptr) {
						loopAction->SetParent(GOparent);
						loopAction->Update(time);
					}
					else {
						throw std::runtime_error("Loop action is not an Action at index: " + actionIdx);
					}
				}
			}
			incrementAction->Update(time); // increments condition
		}

	}


	/** SetCondition
	 * @brief Resets Condition to new condition key
	 * @param conditionKey: key to attribute to be used as condition
	*/
	void ActionListWhile::SetCondition(const string& conditionKey)
	{
		conditionDatum = nullptr;
		condition = conditionKey;
	}

	/** Signatures
	 * @brief returns the class' Signatures
	 * @return vector of class' Signatures
	*/
	std::vector<Signature> ActionListWhile::Signatures()
	{
		return std::vector<Signature> {
			{ "Condition"s, Datum::DatumType::String, 1, offsetof(ActionListWhile, condition)},
			{ "Preamble"s, Datum::DatumType::Table, 0, 0},
			{ "Increment"s, Datum::DatumType::Table, 0, 0 }
		};
	}


	/** AppendId
	 * @brief Appends ActionListWhile's type id to Ids
	 * @param Ids : vector of type Ids
	 * @return Address of vector of Ids
	*/
	std::vector<RTTI::IdType>* ActionListWhile::AppendId(std::vector<RTTI::IdType>* Ids)
	{
		if (Ids == nullptr) {
			ALWid = NEW std::vector<RTTI::IdType>();
			ALWid->push_back(TypeIdClass());
			return ALWid;
		}
		Ids->push_back(TypeIdClass());
		return Ids;
	}
}
