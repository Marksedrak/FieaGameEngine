#include "pch.h"
#include "Action.h"

using namespace std::string_literals;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(Action);

	/** SetName
	 * @brief Set action's Name
	 * @param name : new Name
	*/
	void Action::SetName(const string& name) {
		Name = name;
	}

	/**
	 * @brief 
	 * @param other 
	 * @return True if both have similar Names and Parents
	*/
	bool Action::operator==(Action* other)
	{
		if (other == this) return true;
		if (other->GOparent != GOparent) return false;
		return (Name == other->GetName());
	}

	/** GetName
	 * @brief Get the action's name
	 * @return Name
	*/
	string& Action::GetName()
	{
		return Name;
	}

	/** SetParent
	 * @brief Sets the Game Object parent of this Action
	 * @param parent: GameObject pointer to parent
	*/
	void Action::SetParent(GameObject* parent)
	{
		GOparent = parent;
	}

	/**
	 * @brief Create signatures to be used by attributed and TypeManager
	 * @return vector of signatures
	*/
	std::vector<Signature> Action::Signatures()
	{
		return std::vector<Signature> {
			{ "Name"s, Datum::DatumType::String, 1, offsetof(Action, Name) }
		};
	}
}
