#pragma once
#include "Attributed.h"
#include "Signature.h"
#include "GameClock.h"

using string = std::string;
using Vec4 = glm::vec4;

namespace Fiea::GameEngine {
	

	struct Transform {
		Vec4 Position;
		Vec4 Rotation;
		Vec4 Scale;
	};

	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public:
		GameObject() : Attributed(TypeIdClass(), nullptr) {};

		// Constructor Override if called from child
		GameObject(std::vector<RTTI::IdType>* childIds) : Attributed(TypeIdClass(), childIds) {};
		virtual ~GameObject() = default;
		
		// Doesn't deal with any of it's own copying or moving
		GameObject(const GameObject& rhs) = default;
		GameObject(GameObject&& rhs) noexcept = default;
		GameObject& operator=(const GameObject& rhs) = default;
		GameObject& operator=(GameObject&& rhs) noexcept = default;
		[[nodiscard]] GameObject* Clone() const override;

		// Takes takes a const reference to a GameTime instance and calls Update on all of its children.
		void Update(const GameTime& time);

		bool AddChild(Scope* child);

		bool RemoveChild(Scope* child);

		// Action Methods

		Datum* Actions(int idx = -1);

		bool CreateAction(const string& className, const string& instanceName);

		string Name;
		Transform ObjTransform;
		// This Boolean is solely for testing Update
		bool Updated = false;
		static std::vector<Signature> Signatures();
	};
}
