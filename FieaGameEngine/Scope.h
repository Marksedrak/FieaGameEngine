#pragma once
#include "RTTI.h"
#include "Datum.h"
#include <unordered_map>

namespace Fiea::GameEngine {
	class Scope : public RTTI {
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		// Default tor
		Scope() : Parent(nullptr), _data(), v_data() {};

		Scope(std::uint32_t initCapacity);

		~Scope();

		Scope(const Scope& other);

		Scope& operator=(const Scope& other);

		Scope(Scope&& other) noexcept;

		Scope& operator=(Scope&& rhs) noexcept;

		Datum* Find(const std::string key);
		const Datum* Find(const std::string key) const;

		Datum* Search(const std::string key, Scope** scope = nullptr);
		const Datum* Search(const std::string key, const Scope** scope = nullptr) const;

		Datum& Append(const std::string key);

		Scope& AppendScope(const std::string key, Scope* s = nullptr);

		void Adopt(Scope& scope, std::string key);

		Scope* GetParent() { return Parent; };
		const Scope* GetParent() const { return Parent; };

		void SetParent(Scope* parent) { Parent = parent; };

		Datum& operator[](std::string key);

		Datum& operator[](std::uint32_t idx);

		bool operator==(const Scope& scope) const;

		bool operator!=(const Scope& scope) const;

		Datum* FindContainedScope(const Scope*, std::uint32_t& idx);

		bool isEmpty();

		size_t GetCapacity();
		const size_t GetCapacity() const;

		size_t GetSize() { return v_data.size(); };
		const size_t GetSize() const { return v_data.size(); };

		std::string ToString() const override;
		//bool Equals(const RTTI* rhs) const override;

		Scope* Orphan();

		void Clear();

		virtual Scope* Clone() const;

		bool isAncestorOf(Scope* scope);
		bool isDescendantOf(Scope* scope);

	private:
		std::unordered_map<std::string, Datum> _data;
		std::vector<Datum*>v_data;
		Scope* Parent;
	};
}