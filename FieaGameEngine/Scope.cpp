#include "pch.h"
#include "Scope.h"
#include <sstream>

namespace Fiea::GameEngine{
	RTTI_DEFINITIONS(Scope);

	/** Capacity constructor (Parameterized Constructor)
	 * @brief Constructs a scope with _data and v_data containing an initial capacity
	 * of initCapactiy and sets the Parent to nullptr
	 * @param std::uint32_t initCapacity: initial capacity of Scope's content
	*/
	Scope::Scope(std::uint32_t initCapacity){
		_data.reserve(initCapacity);
		v_data.reserve(initCapacity);
		Parent = nullptr;
	};

	// Destructor
	Scope::~Scope() {
		Clear();
		// Orphan Self
		Orphan();
	}

	/**
	 * @brief Deep copy constructor
	 * @param other 
	 */
	Scope::Scope(const Scope& other) {
		// Deep copy other Scope
		Parent = nullptr;
		_data = other._data;
		v_data = other.v_data;
		for (const auto& pair : other._data) {
			if (pair.second._type == Datum::DatumType::Table) {
				const Datum& datum = pair.second;
				Datum* thisDat = Find(pair.first);
				assert(thisDat != nullptr);
				for (size_t i = 0; i < datum.Size(); ++i) {
					Scope* newScope = datum.GetScope(i)->Clone();
					newScope->Parent = this;
					thisDat->Set(i, newScope);
				}
				// Update v_data to maintain the same order
				for (size_t i = 0; i < v_data.size(); ++i) {
					// since v_data's Datum*s still point to other's Datums
					// use that to recreate the order
					if (v_data[i] == &datum) {
						v_data[i] = thisDat;
					}
				}
			}
			else {
				Datum& dt = Append(pair.first);
				dt = pair.second;

				// Update v_data to maintain the same order
				for (size_t i = 0; i < v_data.size(); ++i) {
					// since v_data's Datum*s still point to other's Datums
					// use that to recreate the order
					if (v_data[i] == &pair.second) {
						v_data[i] = &dt;
					}
				}
			}
		}

	};


	/**
	 * @brief Copy assignment operator
	 * @param rhs 
	 * @return Copy of rhs
	 */
	Scope& Scope::operator=(const Scope& rhs) {
		// Guard against self assignment
		if (rhs == *this) {
			return *this;
		}
		// Clears the current content of this Scope
		Clear();
		// Copy Scope Contents;
		_data = rhs._data;
		v_data = rhs.v_data;
		for (const auto& pair : rhs._data) {
			if (pair.second._type == Datum::DatumType::Table) {
				const Datum& datum = pair.second;
				Datum* thisDat = Find(pair.first);
				assert(thisDat != nullptr);
				for (size_t i = 0; i < datum.Size(); ++i) {
					Scope* newScope = datum.GetScope(i)->Clone();
					newScope->Parent = this;
					thisDat->Set(i, newScope);
				}
				// Update v_data to maintain the same order
				for (size_t i = 0; i < v_data.size(); ++i) {
					// since v_data's Datum*s still point to other's Datums
					// use that to recreate the order
					if (v_data[i] == &datum) {
						v_data[i] = thisDat;
					}
				}
			}
			else {
				Datum& dt = Append(pair.first);
				dt = pair.second;

				// Update v_data to maintain the same order
				for (size_t i = 0; i < v_data.size(); ++i) {
					// since v_data's Datum*s still point to other's Datums
					// use that to recreate the order
					if (v_data[i] == &pair.second) {
						v_data[i] = &dt;
					}
				}
			}
		}
		return *this;
	}


	/**
	 * @brief Move Constructor
	 * @param other 
	 */
	Scope::Scope(Scope&& other) noexcept : _data(std::move(other._data)), v_data(std::move(other.v_data)), Parent(nullptr) {
		for (const auto& pair : _data) {
			if (pair.second._type == Datum::DatumType::Table) {
				Datum* datum = Find(pair.first);
				for (size_t x = 0; x < datum->Size(); ++x) {
					datum->GetScope(x)->Parent = this;
				}
			}
		}
	}


	/**
	 * @brief Move assignment operator
	 * @param rhs 
	 * @return Moved rhs Scope
	 */
	Scope& Scope::operator=(Scope&& rhs) noexcept {
		_data = std::move(rhs._data);
		v_data = std::move(rhs.v_data);
		Parent = nullptr;
		for (const auto& pair : _data) {
			if (pair.second._type == Datum::DatumType::Table) {
				Datum* datum = Find(pair.first);
				for (size_t x = 0; x < datum->Size(); ++x) {
					datum->GetScope(x)->Parent = this;
				}
			}
		}
		return *this;
	}

	/** Clone
	 * @brief Creates a Clone of the current Scope 
	 * @return a Scope reference to a new but identical Scope
	*/
	Scope* Scope::Clone() const{
		// Create New
		return NEW Scope(*this);
	};

	/** Find
	 * @brief Searches the current scope's hash map for the key and returns the corresponding Datum 
	 * @param key 
	 * @return Datum* pointing to the Datum associated with the key, or nullptr if the key is not in the map.
	*/
	Datum* Scope::Find(const std::string key)
	{
		// Looks into _data for the key
		for (auto& pair : _data) {
			if (pair.first == key) {
				return &pair.second;
			}
		}
		return nullptr;
	}

	// const Find
	const Datum* Scope::Find(const std::string key) const
	{
		// Looks into _data for the key
		for (auto& pair : _data) {
			if (pair.first == key) {
				return &pair.second;
			}
		}
		return nullptr;
	}

	/** Search
	 * @brief A more advanced form of Find which looks through the current scope and it's ancestros for the key
	 * @param key : const std::string
	 * @param scope : Scope** used as output if a scope is provided to indicate the containing scope of the Datum if found
	 * @return Datum* to Datum associated with key, and if Scope** is not nullptr it is populated with the containing scope
	*/
	Datum* Scope::Search(const std::string key, Scope** scope) {
		// Uses Find to try and find Datum value with Key in this scope
		Datum* d = Find(key);
		if (d == nullptr) {
			// if this scope has a parent have them call search
			if (GetParent() != nullptr) {
				d = Parent->Search(key, scope);
				return d;
			}
			else {
				return nullptr;
			}
		}
		else {
			if (scope != nullptr) {
				*scope = this;
			}
			return d;

		}
	};

	const Datum* Scope::Search(const std::string key, const Scope** scope) const{
		// Uses Find to try and find Datum value with Key in this scope
		const Datum* d = Find(key);
		if (d == nullptr) {
			// if this scope has a parent have them call search
			if (GetParent() != nullptr) {
				d = Parent->Search(key, scope);
				return d;
			}
			else {
				return nullptr;
			}
		}
		else {
			if (scope != nullptr) {
				*scope = this;
			}
			return d;

		}
	};

	/** Append
	 * @brief Takes the key, tries to find it in the hashmap and returns it or if it doesn't exist, creates a new one and adds it
	 * to the map with given key. Returns created Datum
	 * @param key : std::string 
	 * @return Datum& of existing Datum or newly constructed Datum inserted in map
	*/
	Datum& Scope::Append(const std::string key) {
		// Looks into _data for the key
		auto it = _data.find(key);
		if (it != _data.end()) {
			return it->second;
		}
		else {
			Datum newDatum;
			auto temp = _data.insert(std::make_pair(key, newDatum));
			v_data.push_back(&temp.first->second);
			return temp.first->second;
		}
	}

	/**
	 * @brief Treats Scope as a hash map and returns Datum ref associated with key
	 * @param key : const std::string
	 * @return Datum&
	*/
	Datum& Scope::operator[](const std::string key) {
		return Append(key);
	}

	/**
	 * @brief Treats Scope as a vector, and returns Datum at index idx
	 * @param idx
	 * @return Datum&
	*/
	Datum& Scope::operator[](std::uint32_t idx) {
		return *v_data[idx];
	}

	/** AppendScope:
	 * @brief Appends a Scope into the Datum associated with the key, if it does not exist it Creates a new one
	 * @param key : std::string
	 * @return Scope&
	*/
	Scope& Scope::AppendScope(const std::string key, Scope* s) {
		Datum& dt = Append(key);
		if (s == nullptr) {
			// If type is already set to Table then it's not a new Datum
			if (dt._type == Datum::DatumType::Table) {
				Scope* sc = NEW Scope;
				dt.Push(sc);
				sc->Parent = this;
				return *sc;
			}
			else if (dt._type == Datum::DatumType::Unknown) {
				Scope* sc = NEW Scope;
				dt.SetType(sc);
				dt.Push(sc);
				sc->Parent = this;
				return *sc;
			}
			else {
				throw std::invalid_argument("Datum is not of type: Table");
			}
		}
		else {
			// If type is already set to Table then it's not a new Datum
			if (dt._type == Datum::DatumType::Table) {
				dt.Push(s);
				s->Parent = this;
				return *s;
			}
			else if (dt._type == Datum::DatumType::Unknown) {
				dt.SetType(s);
				dt.Push(s);
				s->Parent = this;
				return *s;
			}
			else {
				throw std::invalid_argument("Datum is not of type: Table");
			}
		}
	}

	/** Adopt
	 * @brief Appends a Scope into this scope's children
	 * @param Scope& scope: Reference to scope you want to Adopt
	 * @param std::string key: Key for the Datum you want to Append the Adopted Scope to
	*/
	void Scope::Adopt(Scope& scope, const std::string key) {
		if (!isAncestorOf(&scope)) { // Prevents Circualar parentage
			if (scope.Parent != nullptr) { // If not allready orphaned from parent
				scope.Orphan();
			}
			// Add scope to current Scope
			Datum* ds = Find(key);
			if (ds == nullptr) {
				Datum& newDatum = Append(key);
				newDatum.Push(&scope);
				scope.Parent = this;
			}
			else {
				ds->Push(&scope);
				v_data.push_back(ds);
				scope.Parent = this;
			}
		}
	}

	/**
	 * @brief Checks if the 
	 * @return 
	*/
	bool Scope::isEmpty() {
		if (_data.size() == 0 && v_data.size() == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * @brief Returns a string representation of the Scope
	 * @return std::string
	*/
	std::string Scope::ToString() const {
		std::stringstream ss;
		for (size_t i = 0; i < v_data.size(); ++i) {
			ss << v_data[i]->ToString();
			ss << " // ";
		}
		return ss.str();
	}

	size_t Scope::GetCapacity() {
		return v_data.capacity();
	};
	const size_t Scope::GetCapacity() const {
		return v_data.capacity();
	}

	/** Comparison Operator
	 * @brief Compares two scopes 
	 * @param const Scope& scope: scope to compare to left-hand side scope
	 * @return true if equal, false otherwise
	*/
	bool Scope::operator==(const Scope& scope) const{
		if (GetSize() != scope.GetSize()) {
			return false;
		}
		else {
			for (size_t i = 0; i < GetSize(); ++i) {
				if (v_data[i]->_type != Datum::DatumType::Table && scope.v_data[i]->_type != Datum::DatumType::Table) {
					if (*v_data[i] != *scope.v_data[i]) {
						return false;
					}
				}
				else {
					for (size_t j = 0; j < v_data[i]->Size(); ++j) {
						if (*v_data[i]->GetScope(j) != *scope.v_data[i]->GetScope(j)) {
							return false;
						}
					}
				}
			}
			return true;
		}
	};

	/** NOT Comparison operator
	 * @brief Compares if both scopes are different (opposite of operator==)
	 * @param const Scope& scope: scope to compare
	 * @return true if not equal, false otherwise
	*/
	bool Scope::operator!=(const Scope& scope) const {
		return !operator==(scope);
	}

	/** FindContainedScope
	 * @brief Finds Scope inside current Scope and returns Datum * of pointer containing the scope as well as index
	 * @param const Scope* scope: Scope trying to find in current Scope
	 * @param std::uint32_t idx: index of Scope in it's Datum
	 * @return Datum *: datum pointer of containing Datum; idx: index of Scope in returned Datum
	*/
	Datum* Scope::FindContainedScope(const Scope* scope, std::uint32_t& idx) {
		// Loops through all Datums in Scope
		for (std::uint32_t i = 0; i < v_data.size(); ++i) {
			if (v_data[i]->_type == Datum::DatumType::Table) { // If Datum is of type Table
				// Loop through Scopes in Datum
				for (std::uint32_t j = 0; j < v_data[i]->Size(); ++j) {
					// If Scope was found return index(idx) and Datum*
					if (v_data[i]->GetScope(j) == scope) {
						idx = j;
						return v_data[i];
					}
				}
			}
		}
		return nullptr;
	};

	/** Orphan
	 * @brief Orphan's this Scope from it's Parent
	 * @param scope 
	 * @return 
	*/
	Scope* Scope::Orphan() {
		if (Parent == nullptr) {
			return this;
		}
		else {
			for (size_t i = 0; i < Parent->v_data.size(); ++i) {
				if (Parent->v_data[i]->_type == Datum::DatumType::Table) {
					Datum& temp = *Parent->v_data[i];
					for (size_t j = 0; j < temp.Size(); ++j) {
						if (temp.GetScope(j) == this) {
							temp.RemoveAt(j);
							Parent = nullptr;
							return this;
						}
					}

				}
			}
		}
		return nullptr;
	}

	/** isDescendantOf
	 * @brief Recursively checks the Parents of this scope for the desired scope.
	 * Returns false if nothing is found
	 * @param Scope* scope : Scope to check for as an ancestor of the current scope
	 * @return true if scope was found; false otherwise
	*/
	bool Scope::isDescendantOf(Scope* scope) {
		if (Parent == nullptr) {
			return false;
		}
		if (Parent == scope) {
			return true;
		}
		else {
			return Parent->isAncestorOf(scope);
		}
	}

	/** isAncestorOf
	 * @brief Checks if scope is a descendant of this scope. The inverse of isDescendantOf
	 * @param scope 
	 * @return true if current Scope is an Ancestor of scope
	*/
	bool Scope::isAncestorOf(Scope* scope) {
		return scope->isDescendantOf(this);
	}

	/** Clear
	 * @brief destroys all the children and content of this Scope wiping it clean 
	*/
	void Scope::Clear() {
		for (size_t i = 0; i < v_data.size(); ++i) {
			// Delete all scopes in the Table types
			if (v_data[i]->_type == Datum::DatumType::Table) {
				Scope** scopePtr = static_cast<Scope**>(v_data[i]->_mData);
				while(!v_data[i]->Empty())
				{
					if (scopePtr[0] == nullptr) break;
					delete scopePtr[0];
				}
			}
		}
	}

}