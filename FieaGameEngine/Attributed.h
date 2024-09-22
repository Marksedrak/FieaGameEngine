#pragma once
#include "Scope.h"
#include "TypeManager.h"

namespace Fiea::GameEngine {
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		virtual ~Attributed() {};
		Attributed(const Attributed& other);
		Attributed& operator=(const Attributed& rhs);
		Attributed(Attributed&& other) noexcept;
		Attributed& operator=(Attributed&& rhs) noexcept;

		virtual Attributed* Clone() const = 0;

		virtual bool IsAttribute(const std::string& name) const;
		virtual bool IsPrescribedAttribute(const std::string& name) const;
		virtual bool IsAuxiliaryAttribute(const std::string& name) const;
		virtual Datum& AppendAuxiliaryAttribute(const std::string& name);

	protected:
		explicit Attributed(RTTI::IdType id, std::vector<RTTI::IdType>* childIds = nullptr);

		// PopulateAttribute Helper method
		void PopulateAttribute(RTTI::IdType id);

	};
}

