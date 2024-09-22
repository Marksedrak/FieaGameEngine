#include "pch.h"
#include "GameObject.h"
#include "Action.h"
#include "Factory.h"

using namespace std::string_literals;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(GameObject);

	/** Clone
	 * @brief Clone method to replicate GameObjects 
	 * @return new GameObject pointer
	*/
	GameObject* GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	/** Update
	 * @brief Update method
	 * @param time 
	*/
	void Fiea::GameEngine::GameObject::Update(const GameTime& time)
	{
		if (Actions()->Size() > 0) {
			for (int action = 0; action < (int) Actions()->GetScope()->GetSize(); ++action) {
				Action* currentAction = Actions(action)->GetScope()->As<Action>();
				currentAction->SetParent(this);
				currentAction->Update(time);
			}
		}

		for (int i = 0; i < (int)Find("Children")->Size(); ++i) {
			Scope* childScope = Find("Children")->GetScope();
			GameObject* currentObj = (*childScope)[i].GetScope()->As<GameObject>();
			// No need to check if it is a Game Object since Add Child checks for that
			currentObj->Update(time);
		};
		Updated = true;

	}

	/** AddChild
	 * @brief Adds child to Datum of children
	 * @param child to add to Datum
	 * @return true if successfully added child, false otherwise
	*/
	bool GameObject::AddChild(Scope* child)
	{
		// Check if it is a game object
		GameObject* objectTest = child->As<GameObject>();
		if (objectTest == nullptr) return false;
		// Due to how you can't have named objects directly in an object array in json
		// Object arrays or Table arrays will contain wrapper Objects which contain the named Scope
		Datum* ChildrenDatum = Find("Children"); // Retrieving Children Datum

		// Check for any empty wrapper scopes
		for (int i = 0; i < (int) ChildrenDatum->Size(); ++i) {
			if (ChildrenDatum->GetScope(i)->GetSize() == 0) {
				ChildrenDatum->GetScope(i)->Adopt(*child, objectTest->Name);
				return true;
			}
		}

		// Creating and pushing a new Scope as the wrapper of the child
		Scope* newScope = new Scope; 
		newScope->SetParent(this);
		ChildrenDatum->Push(newScope);
		
		// Wrapper Adopting child
		newScope->Adopt(*child, objectTest->Name);

		Datum* ChildAdded = ChildrenDatum->GetScope(ChildrenDatum->Size() - 1)->Find(objectTest->Name);

		return (ChildAdded != nullptr);
	}

	/** RemoveChild
	 * @brief Removes child from datum of scopes
	 * @param child to be removed
	 * @return true if removed successfully, false if not found or could not remove
	*/
	bool GameObject::RemoveChild(Scope* child)
	{
		uint32_t idx;
		Datum* ChildrenDatum = Find("Children");
		for (int i = 0; i < (int) ChildrenDatum->Size(); ++i) {
			if (ChildrenDatum->GetScope(i)->FindContainedScope(child, idx) != nullptr) {
				Scope* ChildScope = ChildrenDatum->GetScope(i);
				Datum& Child = (*ChildScope)[idx];
				Child.GetScope()->Orphan();
				return true;
			}
		}
		return false;
	}

	/**
	 * @brief Returns Actions Datum, and if given an index will return the specific Datum of the Action at that index
	 * @param idx (optional)
	 * @return Actions Datum or if index provided will retreive the exact Action's Datum at that index
	*/
	Datum* GameObject::Actions(int idx)
	{
		if (idx == -1) {
			return Find("Actions");
		}
		else {
			Datum* dt = Find("Actions");
			Scope* st = dt->GetScope(0);
			return &(*st)[idx];
		}
	}


	/** CreateAction
	 * @brief Creates new action
	 * @param className: Class name
	 * @param instanceName: instance name
	 * @return True if successfully created, otherwise false
	*/
	bool GameObject::CreateAction(const string& className, const string& instanceName) //Instance name could be set to none later
	{
		// Check that both className and instanceName do not contain whitespace or illegal characters
		if (className.find_first_not_of(" \t\n\v\f\r") == std::string::npos && instanceName.find_first_not_of(" \t\n\v\f\r") == std::string::npos) {
			Scope* ActionCreated = FactoryManager<Scope>::Create(className);

			// Check if created Scope was in fact an Action
			Action* action = ActionCreated->As<Action>();
			if (action != nullptr) {
				action->SetParent(this);
			}
			else {
				throw std::invalid_argument("className entered is not an Action");
				return false;
			}

			Adopt(*ActionCreated, "Actions");
			return true;
		}
		else {
			throw std::invalid_argument("className and instanceName cannot contain whitespaces");
			return false;
		}
		return true;
	}

	std::vector<Signature> GameObject::Signatures() {
		return std::vector<Signature> {
			{ "Name"s, Datum::DatumType::String, 1, offsetof(GameObject, Name) },
			{ "Position"s, Datum::DatumType::Vector, 1, offsetof(GameObject, ObjTransform.Position) },
			{ "Rotation"s, Datum::DatumType::Vector, 1, offsetof(GameObject, ObjTransform.Rotation) },
			{ "Scale"s, Datum::DatumType::Vector, 1, offsetof(GameObject, ObjTransform.Scale) },
			{ "Children"s, Datum::DatumType::Table, 0, 0 },
			{ "Actions"s, Datum::DatumType::Table, 0, 0}
		};
	}


}
