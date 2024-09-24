#include "Datum.h"
#include <stdexcept>
#include "typeinfo"

namespace Fiea::GameEngine {
	
	// Parameterized Constructors

	/** Parameterized Single-item constructor
	 * @brief Constructs Datum of value's type, and saves value as it's single item.
	 * @tparam T
	 * @param value
	*/
	template<class T>
	Datum::Datum(T value) {
		// Determine type of value
		SetType(value);
		if (typeSizes[_type] > 0) {
			T* dataPtr = (T*)malloc(typeSizes[_type]);
			new(dataPtr) T(value);
			_mData = dataPtr;
			_DatumSize = _DatumCapacity = 1;
		}
	};

	// Parameterized Constructors with size

	/**
	 * @brief Constructs a Datum of size (size), allocates memory = sizeof(T) * size, and stores (value) in the location of the first item.
	 * @tparam T: Type of value
	 * @param value: value to store in the first element of Datum
	 * @param size: desired inital size of constructed Datum
	*/
	template<class T>
	Datum::Datum(T value, size_t size) {
		// Determine type of value
		SetType(value);
		if (typeSizes[_type] != 0) {
			_mData = (T*)malloc(typeSizes[_type] * size);
			T* T_mData = static_cast<T*>(_mData);
			new(T_mData) T(value);

			_DatumSize = 1;
			_DatumCapacity = size;
		}
	};

	/** Parameterized Constructor with size
	* @brief Constructs Datum of the type String and allocates memory of Size
	* @param value: const char*
	* @param size: size_t
	*/
	template<>
	inline Datum::Datum(const char* value, size_t size) {
		std::string strValue = std::string(value); // transforms const char* to string strValue
		SetType(value); // Sets the DatumType (_type) for the Datum to String
		_mData = (std::string*)malloc(sizeof(std::string) * size); //Allocates memory equal to size of string * size
		if (sizeof(strValue) > 0) {
			new(_mData) std::string(strValue); // Placement new to put strValue in the beginning of Datum (_mData)
			_DatumSize = 1; //Set DatumSize to size
			_DatumCapacity = size; //Set DatumCapacity to size
		}
	}

	/** Push
	 * @brief Adds value at the end of the current Datum
	 * @tparam T
	 * @param value: value added to Datum 
	*/
	template<class T>
	void Datum::Push(T value) {
		if (externalStorage) {
			throw std::runtime_error("Can't manipulate external storage");
		}
		else {
			// Checks if the type is the same as the current Datum or the Datum is empty
			if (_type == Unknown) {
				//Attempt to SetType
				SetType(value);
			}
			// Check if T is of the same type as Datum
			if (CheckType(value)) {
				// If Datum is at capacity, reallocate more memory
				if (_DatumSize == _DatumCapacity) {
					//String Exception
					if (_type == String) {
						size_t size = typeSizes[_type] * ((_DatumCapacity * 2) + 1);
						std::string* _newData = (std::string*)malloc(typeSizes[_type] * ((_DatumCapacity * 2) + 1));
						std::string* ptr = static_cast<std::string*>(_mData);
						// Transfer over all memory to the new reallocated memory
						for (size_t i = 0; i < _DatumSize; ++i) {
							new(_newData + i) std::string(ptr[i]);
						}
						// free string using ~basic_string()
						for (size_t j = 0; j < _DatumSize; ++j) {
							ptr[j].~basic_string();
						}
						free(_mData);
						// Have _mData point to the address of the new memory
						_mData = _newData;
						// Adjust capacity
						_DatumCapacity = ((_DatumCapacity * 2) + 1);
					}
					else {
						// Allocate memory equal to size * (capacity * 2)
						T* _newData = (T*)malloc(typeSizes[_type] * ((_DatumCapacity * 2) + 1));
						T* ptr = static_cast<T*>(_mData);
						// Transfer over all memory to the new reallocated memory
						for (size_t i = 0; i < _DatumSize; ++i) {
							new(_newData + i) T(ptr[i]);
						}
						// free current memory #Make sure there is a version for non POD
						free(_mData);
						// Have _mData point to the address of the new memory
						_mData = _newData;
						// Adjust capacity
						_DatumCapacity = ((_DatumCapacity * 2) + 1);
					}
				}
				Resize(_DatumCapacity);
				// Add new value
				T* ptr = static_cast<T*>(_mData);
				new(ptr + _DatumSize) T(value);
				_DatumSize++;
			}
			else {
				throw std::runtime_error("Value entered does not match with Datum's current type");
			}
		}
	};

	/** Push
	* @brief Specialization of Push to handle const char* as strings
	* @param value: const char*
	*/
	template<>
	inline void Datum::Push(const char* value) {
		if (!externalStorage) {
			if (_type == Unknown) {
				//Attempt to SetType
				SetType(value);
			}
			// Check if T is of the same type as Datum
			if (CheckType(value)) {
				// If Datum is at capacity, reallocate more memory
				if (_DatumSize == _DatumCapacity) {
					size_t size = typeSizes[_type] * ((_DatumCapacity * 2) + 1);
					std::string* _newData = (std::string*)malloc(typeSizes[_type] * ((_DatumCapacity * 2) + 1));
					std::string* ptr = static_cast<std::string*>(_mData);
					// Transfer over all memory to the new reallocated memory
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(_newData + i) std::string(ptr[i]);
					}
					// free string using ~basic_string()
					for (size_t j = 0; j < _DatumSize; ++j) {
						ptr[j].~basic_string();
					}
					free(_mData);
					// Have _mData point to the address of the new memory
					_mData = _newData;
					// Adjust capacity
					_DatumCapacity = ((_DatumCapacity * 2) + 1);
				}
				Resize(_DatumCapacity);
				// Add new value
				std::string s = std::string(value);
				std::string* ptr = static_cast<std::string*>(_mData);
				new(ptr + _DatumSize) std::string(s);
				_DatumSize++;
			}
			else {
				throw std::runtime_error("Value entered does not match with Datum's current type");
			}
		}
		else {
			throw std::runtime_error("Can't manipulate external storage");
		}
	};

	/** Set
	 * @brief Takes an int index and a reference to a value, which it uses to set the element at index
	 * @tparam T 
	 * @param index 
	 * @param valueRef 
	*/
	template<class T>
	void Datum::Set(size_t idx, T& valueRef) {
		// If Setting to an empty Datum, setup the Datum
		if (_DatumSize == 0 && idx == 0) {
			Push(valueRef);
		}
		// Check if index is out of bounds
		if (idx >= _DatumSize) {
			// Throw Exception
			throw std::out_of_range("Index entered is out of Datum's range");
		}
		else {
			// Verify that valueRef is of a supported type
			if (CheckType(valueRef)) {
				// Set the element at index to valueRef
				T* datPtr = static_cast<T*>(_mData);
				datPtr[idx] = valueRef;
			}
			else {
				throw std::runtime_error("valueRef was not of a supported type");
			}
		}
	};

	/**
	 * @brief Checks if the type of value is the same as the Datum's type
	 * @tparam T 
	 * @param value: value to check against Datum type
	 * @return bool: true if type is the same, false otherwise
	*/
	template<class T>
	bool Datum::CheckType(T value) {
		switch (_type){
		case Datum::Int: {
			if constexpr(std::is_same<T, int>::value)
				return true;
			else
				return false;
		}
		case Datum::Float: {
			if constexpr (std::is_same<T, float>::value)
				return true;
			else
				return false;
		}
		case Datum::String: {
			if constexpr((std::is_same<T, std::string>::value) || (std::is_same<T, const char*>::value)) {
				return true;
			}
			else {
				return false;
			}
		}
		case Datum::Vector: {
			if constexpr(std::is_same<T, glm::vec4>::value)
				return true;
			else
				return false;
		}
		case Datum::Matrix: {
			if constexpr (std::is_same<T, glm::mat4>::value) {
				return true;
			}
			else {
				return false;
			}
		}
		case Datum::Table:
		{
			if constexpr (std::is_same<T, Scope*>::value) {
				return true;
			}
			else {
				return false;
			}
		}
		case Datum::Pointer:
		{
			if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value) {
				return true;
			}
			else {
				return false;
			}
		}
		default:
			throw std::runtime_error("Datum was not initialized");
		}
	};


	template<class T>
	void Datum::SetStorage(T* array, int elementNum) {
		if (_type == Unknown || externalStorage) {
			SetType(*array);
			_mData = array;
			_DatumCapacity = elementNum;
			_DatumSize = elementNum;
			externalStorage = true;
		}
		else {
			throw std::runtime_error("Can't set an already initialized Datum as storage");
		}
	};

	template<class T>
	void Datum::SetStorage(T* array, int elementNum, DatumType type) {
		if (_type == Unknown || externalStorage) {
			if (_type == Unknown) {
				_type = type;
				_DatumCapacity = elementNum;
				_DatumSize = elementNum;
				externalStorage = true;
				_mData = array;
			}
			else if(externalStorage && type == _type) {
				_DatumCapacity = elementNum;
				_DatumSize = elementNum;
				_mData = array;
			}
		}
		else {
			throw std::runtime_error("Can't set an already initialized Datum as storage");
		}
	};

	// General template specialized Get method
	template<>
	inline int& Datum::Get(size_t idx) {
		return GetInt(idx);
	}
	
	template<>
	inline const int& Datum::Get(size_t idx) const {
		return GetInt(idx);
	}

	template<>
	inline float& Datum::Get(size_t idx) {
		return GetFloat(idx);
	}
	
	template<>
	inline const float& Datum::Get(size_t idx) const {
		return GetFloat(idx);
	}

	template<>
	inline std::string& Datum::Get(size_t idx){
		return GetString(idx);
	}
	
	template<>
	inline const std::string& Datum::Get(size_t idx) const{
		return GetString(idx);
	}

	template<>
	inline glm::vec4& Datum::Get(size_t idx) {
		return GetVector(idx);
	}

	template<>
	inline const glm::vec4& Datum::Get(size_t idx) const {
		return GetVector(idx);
	}

	template<>
	inline glm::mat4& Datum::Get(size_t idx) {
		return GetMatrix(idx);
	}

	template<>
	inline const glm::mat4& Datum::Get(size_t idx) const {
		return GetMatrix(idx);
	}

	template<>
	inline RTTI*& Datum::Get(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Pointer) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Pointer");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				RTTI** rPtr = static_cast<RTTI**>(_mData);
				return rPtr[idx];
			}
		}
	}


	/** SetType
	 * @brief Determines the type the Datum is containing/will contain based on the type passed to the method
	 * @param type: const type_info& used to determing the type of a variable/input
	*/
	template<class T>
	void Datum::SetType(T value) {
		if (_type != Unknown) {
			throw std::runtime_error("Cannot change Datum type once already set");
		}
		else {
			// Check for Int
			if constexpr(std::is_same<T, int>::value) {
				_type = DatumType::Int;
				return;
			}
			// Check for float
			else if constexpr(std::is_same<T, float>::value) {
				_type = DatumType::Float;
				return;
			}
			// Check for string
			else if constexpr ((std::is_same<T, std::string>::value) || (std::is_same<T, const char*>::value)) {
				_type = DatumType::String;
				return;
			}
			// Check for Vector 4
			else if constexpr(std::is_same<T, glm::vec4>::value) {
				_type = DatumType::Vector;
				return;
			}
			//Check for Matrix 4x4
			else if constexpr(std::is_same<T, glm::mat4>::value) {
				_type = DatumType::Matrix;
				return;
			}
			else if constexpr (std::is_same<T, Scope*>::value) {
				_type = DatumType::Table;
				return;
			}
			else if constexpr (std::is_same<T, RTTI*>::value) {
				_type = DatumType::Pointer;
				return;
			}
			else {
				throw std::runtime_error("Type entered is not supported by current implemntation");
			}
		}
	}

	/** SetTypeByType
	 * @brief Uses the given DatumType, if the Datum hasn't been initialized, to set the Datum type
	 * @param type : desired Datum type from DatumType enum
	 */
	inline void Datum::SetTypeByType(DatumType type) {
		if (_type != Unknown) {
			throw std::runtime_error("Cannot change Datum type once already set");
		}
		else {
			_type = type;
		}
	}


} // End of Datum.inl