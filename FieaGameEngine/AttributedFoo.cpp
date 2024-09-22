#include "pch.h"
#include <vector>
#include "AttributedFoo.h"

using namespace std::string_literals;

	namespace Fiea::GameEngine {
		RTTI_DEFINITIONS(AttributedFoo);

		AttributedFoo* AttributedFoo::Clone() const {
			return NEW AttributedFoo(*this);
		}

		/** IsAttribute
		 * @brief Checks of the name is associated with a key for an existing Datum in this Attributed
		 * @param name: key to check for
		 * @return True the attribute was found, false otherwise
		*/
		bool AttributedFoo::IsAttribute(const std::string& name) const
		{
			const Datum* dt = Find(name);
			if (dt == nullptr) {
				return false;
			}
			return true;
		}

		/** AppendAuxiliaryAttribute
		 * @brief Adds a Datum to the AttributedFoo with the key name and returns that Datum
		 * @param const std::string& name: key to append to Scope
		 * @return Datum created from Append process
		*/
		Datum& AttributedFoo::AppendAuxiliaryAttribute(const std::string& name)
		{
			return Append(name);
		}

		/** IsPrescribedAttribute
		 * @brief Checks if the name is associated with a Prescribed Attribute
		 * @param name : key associated with the prescribed attribute
		 * @return True if found and is a perscribed attribute, false otherwise
		*/
		bool AttributedFoo::IsPrescribedAttribute(const std::string& name) const {
			if (IsAttribute(name)) {
				for (Signature s : Signatures()) {
					if (s.Name == name) {
						return true;
					}
				}
				return false;
			}
			else {
				return false;
			}
		}

		/** IsAuxiliaryAttribute
		 * @brief Checks if attribute is in the Attributed, but is not a prescribed attribute
 		 * @param name : key to check for
		 * @return True if key is associated with an auxiliary attribute, otherwise false
		*/
		bool AttributedFoo::IsAuxiliaryAttribute(const std::string& name) const {
			if (IsAttribute(name)) {
				if (IsPrescribedAttribute(name)) {
					return false;
				}
				else {
					return true;
				}
			}
			else {
				return false;
			}
		}


		std::vector<Signature> AttributedFoo::Signatures() {
			return std::vector<Signature> {
		{ "externalInteger"s, Datum::DatumType::Int, 1, offsetof(AttributedFoo, externalInteger) },
		{ "externalFloat"s, Datum::DatumType::Float, 1, offsetof(AttributedFoo, externalFloat) },
		{ "externalString"s, Datum::DatumType::String, 1, offsetof(AttributedFoo, externalString) },
		{ "externalVector"s, Datum::DatumType::Vector, 1, offsetof(AttributedFoo, externalVector) },
		{ "externalMatrix"s, Datum::DatumType::Matrix, 1, offsetof(AttributedFoo, externalMatrix) },
		{ "externalIntegerArray"s, Datum::DatumType::Int, arraySize, offsetof(AttributedFoo, externalIntegerArray) },
		{ "externalFloatArray"s, Datum::DatumType::Float, arraySize, offsetof(AttributedFoo, externalFloatArray) },
		{ "externalStringArray"s, Datum::DatumType::String, arraySize, offsetof(AttributedFoo, externalStringArray) },
		{ "externalVectorArray"s, Datum::DatumType::Vector, arraySize, offsetof(AttributedFoo, externalVectorArray) },
		{ "externalMatrixArray"s, Datum::DatumType::Matrix, arraySize, offsetof(AttributedFoo, externalMatrixArray) },
			};
		}
	}