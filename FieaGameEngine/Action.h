#pragma once
#include "Attributed.h"
#include "GameClock.h"
#include "GameObject.h"

using string = std::string;

// Abstract Class that extends Attributed
namespace Fiea::GameEngine {
	class Action : public Attributed {
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		Action() : Attributed(TypeIdClass(), nullptr) {};
		Action(std::vector<RTTI::IdType>* childIds) : Attributed(TypeIdClass(), childIds) {};

		virtual ~Action() = default;
		Action(const Action& other) = default;
		Action& operator=(const Action& rhs) = default;
		Action(Action&& other) noexcept = default;
		Action& operator=(Action&& rhs) noexcept = default;
		virtual Action* Clone() const = 0;

		virtual bool operator==(Action* other);

		virtual void Update(GameTime time) = 0;
		void SetName(const string& name);
		string& GetName();
		void SetParent(GameObject* parent);

		static std::vector<Signature> Signatures();

	protected:
		string Name;
		GameObject* GOparent;
	};
}