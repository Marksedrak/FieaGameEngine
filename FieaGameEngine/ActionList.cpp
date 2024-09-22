#include "pch.h"
#include "ActionList.h"

using namespace std::string_literals;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(ActionList);

	// Destructor
	ActionList::~ActionList()
	{
		if (ALid != nullptr) {
			if (!ALid->empty()) {
				delete ALid;
			}
		}
	}

	ActionList* ActionList::Clone() const{
		return NEW ActionList(*this);
	}

	bool ActionList::operator==(ActionList* other)
	{
		// Check if comparing to self
		if (other == this) return true;
		// Check the names are the same
		if (GetName() != other->GetName()) return false;
		// Check they both have the same amount of actions
		if (Find("Actions")->GetScope()->GetSize() != other->Find("Actions")->GetScope()->GetSize()) return false;
		// Now check if each action is the same
		for (int i = 0; i < (int)Find("Actions")->GetScope()->GetSize(); ++i) {
			Action* Action1 = (*Find("Actions")->GetScope())[i].GetScope()->As<Action>();
			Action* Action2 = (*other->Find("Actions")->GetScope())[i].GetScope()->As<Action>();
			if (!(*Action1 == Action2) || Action1 == nullptr || Action2 == nullptr) {
				return false;
			}
		}
		return true;
	}

	/** Update
	 * @brief Calls update on all actions in ActionList
	 * @param time 
	*/
	void ActionList::Update(GameTime time)
	{
		Scope* ActionsListScope = Find("Actions")->GetScope();
		for (int idx = 0; idx < (int)ActionsListScope->GetSize(); ++idx) {
			Action* a = (*ActionsListScope)[idx].GetScope()->As<Action>();
			if (a != nullptr) {
				a->SetParent(GOparent);
				a->Update(time);
			}
			else {
				throw std::invalid_argument("Non-Action detected in Actions at index: " + idx);
			}
		}
	}

	/** AddAction
	 * @brief Adds an Action to the ActionList
	 * @param action : action to be added
	*/
	void ActionList::AddAction(Action* action)
	{
		Datum* ActionsDatum = Find("Actions");
		if (ActionsDatum != nullptr) {
			Find("Actions")->GetScope()->Adopt(*action, action->GetName());
		}
	}

	std::vector<Signature> ActionList::Signatures() {
		return std::vector<Signature> {
			{ "Actions"s, Datum::DatumType::Table, 0, 0 }
		};
	}


	std::vector<RTTI::IdType>* ActionList::AppendId(std::vector<RTTI::IdType>* Ids)
	{
		if (Ids == nullptr) {
			ALid = NEW std::vector<RTTI::IdType>;
			ALid->push_back(TypeIdClass());
			return ALid;
		}
		Ids->push_back(TypeIdClass());
		return Ids;
	}
}
