#pragma once
#include "GameObject.h"

namespace Fiea::GameEngine {

	class Hero : public GameObject
	{
		RTTI_DECLARATIONS(Hero, GameObject);

	public:

		Hero(std::vector<RTTI::IdType>* Ids = nullptr) : GameObject(AppendId(Ids)) {};
		virtual ~Hero();

		string PassiveName;
		string HeroName;
		
		static std::vector<Signature> Signatures();

	private:
		std::vector<RTTI::IdType>* AppendId(std::vector<RTTI::IdType>* Ids);
		std::vector<RTTI::IdType>* HeroID;
	};
}

