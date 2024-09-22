#pragma once

#include <stdexcept>
#include <algorithm>
#include "Attributed.h"
#include "Signature.h"
#include "TypeManager.h"
#include "Foo.h"

namespace Fiea::GameEngine {
	class AttributedFoo final : public Attributed {
		RTTI_DECLARATIONS(AttributedFoo, Attributed);
	
	public:
		
		static const std::size_t arraySize = 5;

		AttributedFoo() : Attributed(TypeIdClass(), nullptr) {}
		AttributedFoo(const AttributedFoo& rhs) = default;
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;
		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;
		virtual ~AttributedFoo() = default;
		[[nodiscard]] AttributedFoo* Clone() const override;

		virtual bool IsAttribute(const std::string& name) const override;
		virtual bool IsPrescribedAttribute(const std::string& name) const override;
		virtual bool IsAuxiliaryAttribute(const std::string& name) const override;
		virtual Datum& AppendAuxiliaryAttribute(const std::string& name) override;

		int externalInteger = 4;
		float externalFloat = 3.6f;
		std::string externalString = "Hi";
		glm::vec4 externalVector = glm::vec4(2, 4, 1, 5);
		glm::mat4 externalMatrix = glm::mat4(2.54f);

		int externalIntegerArray[arraySize] = { 1,4,6,7 };
		float externalFloatArray[arraySize] = { 2.4f, 4.3f, 3.6f };
		std::string externalStringArray[arraySize] = { "Hello", "It's", "Me" };
		glm::vec4 externalVectorArray[arraySize] = { glm::vec4(2, 4, 1, 5), glm::vec4(4, 2, 1, 5) };
		glm::mat4 externalMatrixArray[arraySize] = { glm::mat4(3.6f), glm::mat4(2.2f) };

		//bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override { return Attributed::ToString(); }
		static std::vector<Signature> Signatures();
		
	};
}