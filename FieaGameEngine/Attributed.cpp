#pragma once 

#include "pch.h"
#include "Attributed.h"
#include "Signature.h"

using string = std::string;
using vec4 = glm::vec4;
using mat4 = glm::mat4;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(RTTI::IdType id, std::vector<RTTI::IdType>* childIds)
	{
		PopulateAttribute(id);
		if (childIds != nullptr) {
			for (int idx = 0; idx < (int)childIds->size(); ++idx) {
				PopulateAttribute(childIds->at(idx));
			}
		}
	};

	Attributed::Attributed(const Attributed& other) : Scope(other){
		PopulateAttribute(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& rhs) {
		if (&rhs == this) {
			return *this;
		}
		Scope::operator=(rhs);
		PopulateAttribute(rhs.TypeIdInstance());
		return *this;
	}

	Attributed::Attributed(Attributed&& moveother) noexcept : Scope(std::move(moveother)){
		PopulateAttribute(moveother.TypeIdInstance());
	}

	Attributed& Attributed::operator=(Attributed&& moverhs) noexcept{
		if (&moverhs == this) {
			return *this;
		}
		Scope::operator=(std::move(moverhs));
		PopulateAttribute(moverhs.TypeIdInstance());
		return *this;
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		// check if this is an attribute
		if (Find(name) == nullptr) return false;
		else {
			return true;
		}
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const {
		const std::vector<Signature> sig = TypeManager::get(TypeIdInstance());
		for (Signature s : sig) {
			if (s.Name == name) {
				return true;
			}
		}
		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const {
		if (IsAttribute(name) && !IsPrescribedAttribute(name)) {
			return true;
		}
		else {
			return false;
		}
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		return Append(name);
	}

	void Attributed::PopulateAttribute(RTTI::IdType id) {
		const std::vector<Signature> sig = TypeManager::get(id);
		char* beginPtr = reinterpret_cast<char*>(this);

		// Adding first element containing this pointer
		if (Find("This") == nullptr) {
			Append("This").SetStorage(this, 1, Datum::Pointer);
		}
		for (Signature s : sig) {
			if (s.Offset == 0) {
				// check if cloning
				if (Find(s.Name) == nullptr) {
					Append(s.Name).SetTypeByType(s.Type);
				}
			}
			else {
				Append(s.Name).SetStorage((beginPtr + s.Offset), s.size, s.Type);
			}
		};

	}
}
