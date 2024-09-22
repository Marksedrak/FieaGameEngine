#include "pch.h"
#include "Hero.h"
#include "Signature.h"

using namespace std::string_literals;

namespace Fiea::GameEngine{
	RTTI_DEFINITIONS(Hero);

	Hero::~Hero()
	{
		delete HeroID;
	}

	std::vector<Signature> Hero::Signatures()
	{
		return std::vector<Signature> {
			{ "PassiveName"s, Datum::DatumType::String, 1, offsetof(Hero, PassiveName) },
			{ "HeroName"s, Datum::DatumType::String, 1, offsetof(Hero, HeroName) }
		};
	}

	std::vector<RTTI::IdType>* Hero::AppendId(std::vector<RTTI::IdType>* Ids)
	{
		if (Ids == nullptr) {
			HeroID = new std::vector<RTTI::IdType>();
			HeroID->push_back(TypeIdClass());
			return HeroID;
		}
		Ids->push_back(TypeIdClass());
		return Ids;
	}

}
