#include "pch.h"
#include "Datum.h"
#include "typeinfo"
#include <stdexcept>
#include <regex>
#include <sstream>

#define NULLVEC glm::vec4(0.0f);
#define NULLMAT glm::mat4(0.0f);

namespace Fiea::GameEngine {


	/** Default Constructor
	 * @brief Construct and empty Datum and setting startPointer to nullptr
	*/
	Datum::Datum(): _mData(nullptr) {}

	/** Deconstructor
	 * @brief frees up allocated memory and makes sure Datum is deconstructed correctly
	*/
	Datum::~Datum() {
		if (externalStorage) {
			_mData = nullptr;
			_DatumCapacity = 0;
			_DatumSize = 0;
			externalStorage = false;
			_type = Unknown;
		}
		else {
			switch (_type)
			{
			case Table:
			case Pointer:
			case Int:
			case Float:
			case Vector:
			case Matrix:
				free(_mData); // For all PODs just frees _mData and the contents are dealt with
				break;
			case String: {
				std::string* stringPtr = static_cast<std::string*>(_mData);
				for (size_t i = 0; i < _DatumSize; ++i) {
					(stringPtr + i)->~basic_string();
				}
				free(stringPtr);
				break;
			}
			default:
				break;
			};
		}
	}

	/** Parameterized constructor template specialization for strings
	 * @brief Specialized Parameterized constructor for string types
	 * @param const char* st
	*/
	template<>
	Datum::Datum(const char* value) {
		// Creates a string out of the const char*
		std::string strValue = std::string(value);
		SetType(value); // Sets the DatumType (_type) for the Datum
		
		_mData = (std::string*)malloc(sizeof(std::string)); //Allocate memory for a single string
		new(_mData) std::string(strValue); // Placement new
		_DatumSize++;
		_DatumCapacity++;
	}

	// Copy Constructors
	Datum::Datum(Datum& other): _DatumSize(other._DatumSize), _DatumCapacity(other._DatumCapacity), _type(other._type) {
		if (other.externalStorage) {
			externalStorage = true;
			_mData = other._mData;
		}
		else {
			if (_type != Unknown) {
				// Allocate memory based on the other Datum's capacity
				_mData = malloc(typeSizes[_type] * _DatumCapacity);
				// Copies over content based on type
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Int: {
					int* intPtr = static_cast<int*>(_mData);
					int* otherPtr = static_cast<int*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(intPtr + i) int(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Float: {
					float* floatPtr = static_cast<float*>(_mData);
					float* otherPtr = static_cast<float*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(floatPtr + i) float(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::String: {
					std::string* strPtr = static_cast<std::string*>(_mData);
					std::string* otherPtr = static_cast<std::string*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(strPtr + i) std::string(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Vector: {
					glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
					glm::vec4* otherPtr = static_cast<glm::vec4*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(vecPtr + i) glm::vec4(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Matrix: {
					glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
					glm::mat4* otherPtr = static_cast<glm::mat4*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(matPtr + i) glm::mat4(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Table: {
					Scope** scopePtr = static_cast<Scope**>(_mData);
					Scope** otherPtr = static_cast<Scope**>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(scopePtr + i) Scope*(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Pointer: {
					RTTI** rPtr = static_cast<RTTI**>(_mData);
					RTTI** otherPtr = static_cast<RTTI**>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(rPtr + i) RTTI*(otherPtr[i]);
					}
					break;
				}
				}
			}
		}
	}

	Datum::Datum(const Datum& other) : _DatumSize(other._DatumSize), _DatumCapacity(other._DatumCapacity), _type(other._type) {
		if (other.externalStorage) {
			externalStorage = true;
			_mData = other._mData;
		}
		else {
			if (_type != Unknown) {
				// Allocate memory based on the other Datum's capacity
				_mData = malloc(typeSizes[_type] * _DatumCapacity);
				// Copies over content based on type
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Int: {
					int* intPtr = static_cast<int*>(_mData);
					int* otherPtr = static_cast<int*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(intPtr + (typeSizes[_type] * i)) int(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Float: {
					float* floatPtr = static_cast<float*>(_mData);
					float* otherPtr = static_cast<float*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(floatPtr + (typeSizes[_type] * i)) float(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::String: {
					std::string* strPtr = static_cast<std::string*>(_mData);
					std::string* otherPtr = static_cast<std::string*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(strPtr + (typeSizes[_type] * i)) std::string(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Vector: {
					glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
					glm::vec4* otherPtr = static_cast<glm::vec4*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(vecPtr + (typeSizes[_type] * i)) glm::vec4(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Matrix: {
					glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
					glm::mat4* otherPtr = static_cast<glm::mat4*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(matPtr + (typeSizes[_type] * i)) glm::mat4(otherPtr[i]);
					}
				}
				}
			}
		}
	}

	/**
	 * @brief Move Constructor
	 * @param other: rvalue of Datum 
	*/
	Datum::Datum(Datum&& other) noexcept : _DatumSize(other._DatumSize), _DatumCapacity(other._DatumCapacity), _type(other._type), _mData(other._mData), externalStorage(other.externalStorage) {
		other._mData = nullptr;
		other._DatumCapacity = 0;
		other._DatumSize = 0;
		other._type = Unknown;
		other.externalStorage = false;
	};
	
	/**
	 * @brief Assignment Operator
	 * @param other: rhs Datum
	*/
	void Datum::operator=(const Datum& other){
		if (other.externalStorage) {
			_mData = other._mData;
			_type = other._type;
			_DatumCapacity = other._DatumCapacity;
			_DatumSize = other._DatumSize;
			externalStorage = true;
		}
		else {
			if (_type == Unknown && other._type != Unknown) {
				// Put transfer everything to lhs Datum
				_type = other._type;
				_DatumSize = other._DatumSize;
				_DatumCapacity = other._DatumCapacity;
				// Allocate memory based on the other Datum's capacity
				_mData = malloc(typeSizes[_type] * _DatumCapacity);
				// Copies over content based on type
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Int: {
					int* intPtr = static_cast<int*>(_mData);
					int* otherPtr = static_cast<int*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(intPtr + (typeSizes[_type] * i)) int(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Float: {
					float* floatPtr = static_cast<float*>(_mData);
					float* otherPtr = static_cast<float*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(floatPtr + (typeSizes[_type] * i)) float(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::String: {
					std::string* strPtr = static_cast<std::string*>(_mData);
					std::string* otherPtr = static_cast<std::string*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(strPtr + (typeSizes[_type] * i)) std::string(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Vector: {
					glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
					glm::vec4* otherPtr = static_cast<glm::vec4*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(vecPtr + (typeSizes[_type] * i)) glm::vec4(otherPtr[i]);
					}
					break;
				}
				case Fiea::GameEngine::Datum::Matrix: {
					glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
					glm::mat4* otherPtr = static_cast<glm::mat4*>(other._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(matPtr + (typeSizes[_type] * i)) glm::mat4(otherPtr[i]);
					}
					break;
				}
				}
			}
		}
	}

	// Assignment Operator Overloads
	
	/**
	 * @brief Operator= Overload which initializes or sets Datum to scalar value int i
	 * @param int i: scalar value to be contained by Datum
	*/
	void Datum::operator=(int i) {
		if (!externalStorage) {
			if (_type == Unknown || _type == Int) {
				if (_DatumSize > 0) {
					free(_mData);
				}
				_mData = (int*)malloc(sizeof(int));
				new(_mData) int(i);
				_DatumCapacity = 1;
				_DatumSize = 1;
				if (_type == Unknown) {
					SetType(i);
				}
			}
			else {
				throw std::runtime_error("Datum already initalized and of a different type than Int");
			}
		}
		else {
			throw std::runtime_error("Can change or manipulate external storage");
		}
	}

	/**
	 * @brief Operator= Overload which initializes or sets Datum to scalar value float f
	 * @param float f: scalar value to be contained by Datum
	*/
	void Datum::operator=(float f) {
		if (!externalStorage) {
			if (_type == Unknown || _type == Float) {
				if (_DatumSize > 0) {
					free(_mData);
				}
				_mData = (float*)malloc(sizeof(float));
				new(_mData) float(f);
				_DatumCapacity = 1;
				_DatumSize = 1;
				if (_type == Unknown) {
					SetType(f);
				}
			}
			else {
				throw std::runtime_error("Datum already initalized and of a different type than Float");
			}
		}
		else {
			throw std::runtime_error("Can't manipulate external storage");
		}
	}

	/**
	 * @brief Operator= Overload that initializes or sets Datum to scalar value String s
	 * @param string s: scalar value to be contained by Datum
	*/
	void Datum::operator=(std::string s) {
		if (!externalStorage) {
			if (_type == Unknown || _type == String) {
				if (_DatumSize > 0) {
					std::string* strPtr = static_cast<std::string*>(_mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						strPtr[i].~basic_string();
					}
					free(strPtr);
				}
				_mData = (std::string*)malloc(sizeof(std::string));
				new(_mData) std::string(s);
				_DatumCapacity = 1;
				_DatumSize = 1;
				if (_type == Unknown) {
					SetType(s);
				}
			}
			else {
				throw std::runtime_error("Datum already initalized and of a different type than String");
			}
		}
		else {
			throw std::runtime_error("Can't manipulate external storage");
		}
	}

	/**
	 * @brief Operator= Overload that initializes or sets Datum to scalar value Vector v
	 * @param glm::vec4 v: 4-D Vector
	*/
	void Datum::operator=(glm::vec4 v) {
		if (!externalStorage) {
			if (_type == Unknown || _type == Vector) {
				if (_DatumSize > 0) {
					free(_mData);
				}
				_mData = (glm::vec4*)malloc(sizeof(glm::vec4));
				new(_mData) glm::vec4(v);
				_DatumCapacity = 1;
				_DatumSize = 1;
				if (_type == Unknown) {
					SetType(v);
				}
			}
			else {
				throw std::runtime_error("Datum already initalized and of a different type than Vector");
			}
		}
		else {
			throw std::runtime_error("Can't manipulate external storage");
		}
	}

	/**
	 * @brief Operator= Overload that initializes or sets Datum to scalar value Matrix m
	 * @param glm::mat4 m: Matrix 4x4
	*/
	void Datum::operator=(glm::mat4 m) {
		if (!externalStorage) {
			if (_type == Unknown || _type == Matrix) {
				if (_DatumSize > 0) {
					free(_mData);
				}
				_mData = (glm::mat4*)malloc(sizeof(glm::mat4));
				new(_mData) glm::mat4(m);
				_DatumCapacity = 1;
				_DatumSize = 1;
				if (_type == Unknown) {
					SetType(m);
				}
			}
			else {
				throw std::runtime_error("Datum already initalized and of a different type than Matrix");
			}
		}
		else {
			throw std::runtime_error("Can't manipulate external storage");
		}
	}


	// Move Assignment

	void Datum::operator=(Datum&& other) noexcept { //Ask if this should be able to change types
		if (!externalStorage) {
			if (_type == Unknown || _type == other._type) {
				// If Datum contains items, clear it out
				if (_DatumSize > 0) {
					// Handle string proper disposal
					if (_type == String) {
						// Create string pointer to navigate string correctly
						std::string* strPtr = static_cast<std::string*>(_mData);
						for (size_t i = 0; i < _DatumSize; ++i) {
								strPtr[i].~basic_string();
						}
						free(strPtr);
					}
					else {
						free(_mData);
					}
				}
				// Move everything to current Datum
				_mData = other._mData;
				_DatumCapacity = other._DatumCapacity;
				_DatumSize = other._DatumSize;
				if (_type == Unknown) {
					_type = other._type;
				}

				// Empty out other
				other._mData = nullptr;
				other._DatumCapacity = 0;
				other._DatumSize = 0;
				other._type = Unknown;
			}
		}
	};

	// Equality operator
	bool Datum::operator==(Datum& rhs) {
		if (_type == rhs._type) {
			if (_DatumSize == rhs._DatumSize) {
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Unknown:
					return true;
				case Fiea::GameEngine::Datum::Int:
				{
					int* intPtr = static_cast<int*>(_mData);
					int* otherintPtr = static_cast<int*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (intPtr[i] != otherintPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Float:
				{
					float* floatPtr = static_cast<float*>(_mData);
					float* otherfPtr = static_cast<float*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (floatPtr[i] != otherfPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::String:
				{
					std::string* strPtr = static_cast<std::string*>(_mData);
					std::string* otherStrPtr = static_cast<std::string*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (strPtr[i] != otherStrPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Vector:
				{
					glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
					glm::vec4* otherVecPtr = static_cast<glm::vec4*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (vecPtr[i] != otherVecPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Matrix:
				{
					glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
					glm::mat4* otherMatPtr = static_cast<glm::mat4*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (matPtr[i] != otherMatPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Table:
				{
					Scope** scopePtr = static_cast<Scope**>(_mData);
					Scope** otherScopePtr = static_cast<Scope**>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (scopePtr[i] != otherScopePtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Pointer:
					// Special thing for RTTI**
					return false;
				default:
					throw std::invalid_argument("Invalid Datum Type");
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	// Equal Comparison operator
	bool Datum::operator==(const Datum& rhs) const{
		if (_type == rhs._type) {
			if (_DatumSize == rhs._DatumSize) {
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Unknown:
					return true;
				case Fiea::GameEngine::Datum::Int:
				{
					int* intPtr = static_cast<int*>(_mData);
					int* otherintPtr = static_cast<int*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (intPtr[i] != otherintPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Float:
				{
					float* floatPtr = static_cast<float*>(_mData);
					float* otherfPtr = static_cast<float*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (floatPtr[i] != otherfPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::String:
				{
					std::string* strPtr = static_cast<std::string*>(_mData);
					std::string* otherStrPtr = static_cast<std::string*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (strPtr[i] != otherStrPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Vector:
				{
					glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
					glm::vec4* otherVecPtr = static_cast<glm::vec4*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (vecPtr[i] != otherVecPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Matrix:
				{
					glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
					glm::mat4* otherMatPtr = static_cast<glm::mat4*>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (matPtr[i] != otherMatPtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Table:
				{
					Scope** scopePtr = static_cast<Scope**>(_mData);
					Scope** otherScopePtr = static_cast<Scope**>(rhs._mData);
					for (size_t i = 0; i < _DatumSize; ++i) {
						if (scopePtr[i] != otherScopePtr[i]) {
							return false;
						}
					}
					return true;
				}
				case Fiea::GameEngine::Datum::Pointer:
					// Special thing for RTTI**
					return false;
				default:
					throw std::invalid_argument("Invalid Datum Type");
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	bool Datum::operator!=(Datum& rhs) {
		return !operator==(rhs);
	}


	/** GetType
	* @brief retruns a string with the type that the Datum contains
	* @return string with Datum's contained type
	*/
	const std::string Datum::GetType() const{
		std::string type;
		switch (_type)
		{
		case Fiea::GameEngine::Datum::Unknown:
			type = "Unknown";
			break;
		case Fiea::GameEngine::Datum::Int:
			type = "Int";
			break;
		case Fiea::GameEngine::Datum::Float:
			type = "Float";
			break;
		case Fiea::GameEngine::Datum::String:
			type = "String";
			break;
		case Fiea::GameEngine::Datum::Vector:
			type = "Vector";
			break;
		case Fiea::GameEngine::Datum::Matrix:
			type = "Matrix";
			break;
		default:
			break;
		}
		return type;
	};


	/** CheckType
	 * @brief Checks if the current Datum type matches with the given Datum type
	 * @param type 
	 * @return True if they match, false otherwise
	*/
	bool Datum::CheckType(DatumType type){
		return (_type == type);
	}

	// Empty

	/**
	 * @brief empty checks if the Datum is currently empty (size = 0)
	 * @return bool: true if size = 0, false otherwise
	*/
	bool Datum::Empty() {
		if (_DatumSize == 0) {
			return true;
		}
		return false;
	}

	/**
	 * @brief Retrieves the Int at index idx
	 * @param idx: size_t index
	 * @return int&: int reference
	*/
	int& Datum::GetInt(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Int) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Int");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				int* intPtr = static_cast<int*>(_mData);
				return intPtr[idx];
			}
		}
	};
	const int& Datum::GetInt(size_t idx) const{
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Int) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Int");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				int* intPtr = static_cast<int*>(_mData);
				return intPtr[idx];
			}
		}
	};

	/**
	 * @brief Retrieves the float at index idx
	 * @param idx: size_t index
	 * @return float&: float reference
	*/
	float& Datum::GetFloat(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Float) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Float");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				float* floatPtr = static_cast<float*>(_mData);
				return floatPtr[idx];
			}
		}
	};
	const float& Datum::GetFloat(size_t idx) const {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Float) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Float");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				float* floatPtr = static_cast<float*>(_mData);
				return floatPtr[idx];
			}
		}
	}

	/**
	 * @brief Retrieves the string at index idx
	 * @param idx: size_t index
	 * @return std::string&: string reference
	*/
	std::string& Datum::GetString(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != String) {
			throw std::runtime_error("Datum is either uninitialized or not of type: String");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				std::string* strPtr = static_cast<std::string*>(_mData);
				return strPtr[idx];
			}
		}
	}
	const std::string& Datum::GetString(size_t idx) const {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != String) {
			throw std::runtime_error("Datum is either uninitialized or not of type: String");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				std::string* strPtr = static_cast<std::string*>(_mData);
				return strPtr[idx];
			}
		}
	}

	/**
	 * @brief Retrieves the 4D Vector at index idx
	 * @param idx: size_t index
	 * @return glm::vec4&: Vec4 reference
	*/
	glm::vec4& Datum::GetVector(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Vector) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Vector");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
				return vecPtr[idx];
			}
		}
	}
	const glm::vec4& Datum::GetVector(size_t idx) const {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Vector) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Vector");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
				return vecPtr[idx];
			}
		}
	};

	/**
	 * @brief Retrieves the matrix at index idx
	 * @param idx: size_t index
	 * @return glm::mat4&: Matrix 4x4 reference
	*/
	glm::mat4& Datum::GetMatrix(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Matrix) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Matrix");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
				return matPtr[idx];
			}
		}
	}
	const glm::mat4& Datum::GetMatrix(size_t idx) const {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Matrix) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Matrix");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
				return matPtr[idx];
			}
		}
	};

	Scope* Datum::GetScope(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Table) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Matrix");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				Scope** scopePtr = static_cast<Scope**>(_mData);
				return scopePtr[idx];
			}
		}
	}

	const Scope* Datum::GetScope(size_t idx) const{
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Table) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Matrix");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				Scope** scopePtr = static_cast<Scope**>(_mData);
				return scopePtr[idx];
			}
		}
	}

	RTTI* Datum::GetPointer(size_t idx) {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Table) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Matrix");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				RTTI** rttiPtr = static_cast<RTTI**>(_mData);
				return rttiPtr[idx];
			}
		}
	}

	const RTTI* Datum::GetPointer(size_t idx) const {
		// If _type is Unknown throws exception and returns
		if (_type == Unknown || _type != Table) {
			throw std::runtime_error("Datum is either uninitialized or not of type: Matrix");
		}
		else {
			if (idx >= _DatumSize) { //Checks idx/index is within range
				throw std::out_of_range("index out of range");
			}
			else {
				RTTI** rttiPtr = static_cast<RTTI**>(_mData);
				return rttiPtr[idx];
			}
		}
	}

	/** Pop
	 * @brief Removes the very last element of the Datum
	*/
	void Datum::Pop() {
		if (!externalStorage) {
			// Check if _DatumSize is more than 0
			if (!Empty()) {
				// Remove the last element of Datum
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Int:
				{
					// get an Int pointer version of _mData
					int* intPtr = static_cast<int*>(_mData);
					intPtr[_DatumSize - 1] = NULL;
					--_DatumSize;
					break;
				}
				case Fiea::GameEngine::Datum::Float:
				{
					// get an Float pointer version of _mData
					float* fPtr = static_cast<float*>(_mData);
					fPtr[_DatumSize - 1] = NULL;
					--_DatumSize;
					break;
				}
				case Fiea::GameEngine::Datum::String:
				{
					// get an String pointer version of _mData
					std::string* strPtr = static_cast<std::string*>(_mData);
					(strPtr + (_DatumSize - 1))->~basic_string();
					--_DatumSize;
					break;
				}
				case Fiea::GameEngine::Datum::Vector:
				{
					// get an Vector pointer version of _mData
					glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
					vecPtr[_DatumSize - 1] = glm::vec4(0.0f);
					--_DatumSize;
					break;
				}
				case Fiea::GameEngine::Datum::Matrix:
				{
					// get an Vector pointer version of _mData
					glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
					matPtr[_DatumSize - 1] = glm::mat4(0.0f);
					--_DatumSize;
					break;
				}
				case Fiea::GameEngine::Datum::Table:
					break;
				case Fiea::GameEngine::Datum::Pointer:
					break;
				default:
					break;
				}
			}
			else {
				throw std::out_of_range("Trying to Pop from an empty Datum");
			}
		}
		else {
			throw std::runtime_error("Can't manipulate external storage");
		}
	};


	/**
	 * @brief SetFromString sets an element in Datum at index (index) with the value represented with the string s
	 * @param index 
	 * @param s 
	*/
	void Datum::SetFromString(size_t idx, std::string s) {
		// Verify index is not out of bounds
		if(idx < _DatumSize){
			if (_type == Unknown) {
				// Identify type
				if (s.substr(0, 4) == "Vec4" || s.substr(0, 4) == "vec4") {
					_type = Vector;
				}
				if (s.substr(0, 4) == "Mat4" || s.substr(0, 4) == "mat4") {
					_type = Matrix;
				}
				// Check if int by checking if all the chars in s are digits
				bool isInt = true;
				for (char c : s) {
					if (!std::isdigit(c)) {
						isInt = false;
					}
				}
				if (isInt) {
					_type = Int;
				}

				// Check for float
				if (std::regex_match(s, std::regex("[0-9]+\\.[0-9]+[f]?"))) {
					_type = Float;
				}

				// If none then string
				if (_type == Unknown) {
					_type = String;
				}
			}
			switch (_type)
			{
			case Fiea::GameEngine::Datum::Int:{
				if (std::regex_match(s, std::regex("[0-9]*"))) {
					int value = std::stoi(s);
					Set(idx, value);
				}
				else {
					throw std::runtime_error("Invalid string entered");
				}
				break;
			}
			case Fiea::GameEngine::Datum::Float:{
				if (std::regex_match(s, std::regex("[0-9]+\\.[0-9]+[f]?"))) {
					float f = stof(s);
					Set(idx, f);
				}
				else {
					throw std::runtime_error("Invalid string entered");
				}
				break;
			}
			case Fiea::GameEngine::Datum::String:
			{
				// Can't throw exception since 
				Set(idx, s);
				break;
			}
			case Fiea::GameEngine::Datum::Vector:
			{
				float vec1, vec2, vec3, vec4;
				if (sscanf_s(s.c_str(), "Vec4(%f, %f, %f, %f)", &vec1, &vec2, &vec3, &vec4) == 4 || sscanf_s(s.c_str(), "vec4(%f, %f, %f, %f)", &vec1, &vec2, &vec3, &vec4) == 4) {
					// Parse the string
					glm::vec4 vec(vec1, vec2, vec3, vec4);
					Set(idx, vec);
				}
				else {
					throw std::runtime_error("Invalid string entered");
				}
				break;
			}
			case Fiea::GameEngine::Datum::Matrix: 
			{
				float mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10, mat11, mat12, mat13, mat14, mat15, mat16;
				if (sscanf_s(s.c_str(), "Mat4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &mat1, &mat2, &mat3, &mat4, &mat5, &mat6, &mat7, &mat8, &mat9, &mat10, &mat11, &mat12, &mat13, &mat14, &mat15, &mat16) == 16 || sscanf_s(s.c_str(), "mat4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &mat1, &mat2, &mat3, &mat4, &mat5, &mat6, &mat7, &mat8, &mat9, &mat10, &mat11, &mat12, &mat13, &mat14, &mat15, &mat16) == 16) {
					// Parse the string
					glm::mat4 mat(mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10, mat11, mat12, mat13, mat14, mat15, mat16);

					Set(idx, mat);
				}
				else { throw std::runtime_error("Invalid string entered"); }
				break;
			}
			case Fiea::GameEngine::Datum::Table:
				break;
			case Fiea::GameEngine::Datum::Pointer:
				break;
			default:
				break;
			}
		}
		else {
			if (_DatumSize == idx) {
				if (_type == Unknown) {
					// Identify type
					if (s.substr(0, 4) == "Vec4" || s.substr(0, 4) == "vec4") {
						_type = Vector;
					}
					if (s.substr(0, 4) == "Mat4" || s.substr(0, 4) == "mat4") {
						_type = Matrix;
					}
					// Check if int by checking if all the chars in s are digits
					bool isInt = true;
					for (char c : s) {
						if (!std::isdigit(c)) {
							isInt = false;
							break;
						}
					}
					if (isInt) {
						_type = Int;
					}

					// Check for float
					if (std::regex_match(s, std::regex("[0-9]+\\.[0-9]+[f]?"))) {
						_type = Float;
					}

					// If none then string
					if (_type == Unknown) {
						_type = String;
					}
				}
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Int: {
					if (std::regex_match(s, std::regex("[0-9]*"))) {
						int value = std::stoi(s);
						Push(value);
					}
					else {
						throw std::runtime_error("Invalid string entered");
					}
					break;
				}
				case Fiea::GameEngine::Datum::Float: {
					if (std::regex_match(s, std::regex("[0-9]+\\.[0-9]+[f]?"))) {
						float f = stof(s);
						Push(f);
					}
					else {
						throw std::runtime_error("Invalid string entered");
					}
					break;
				}
				case Fiea::GameEngine::Datum::String:
				{
					// Can't throw exception since 
					Push(s);
					break;
				}
				case Fiea::GameEngine::Datum::Vector:
				{
					float vec1, vec2, vec3, vec4;
					if (sscanf_s(s.c_str(), "Vec4(%f, %f, %f, %f)", &vec1, &vec2, &vec3, &vec4) == 4 || sscanf_s(s.c_str(), "vec4(%f, %f, %f, %f)", &vec1, &vec2, &vec3, &vec4) == 4) {
						// Parse the string
						glm::vec4 vec(vec1, vec2, vec3, vec4);
						Push(vec);
					}
					else {
						throw std::runtime_error("Invalid string entered");
					}
					break;
				}
				case Fiea::GameEngine::Datum::Matrix:
				{
					float mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10, mat11, mat12, mat13, mat14, mat15, mat16;
					if (sscanf_s(s.c_str(), "Mat4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &mat1, &mat2, &mat3, &mat4, &mat5, &mat6, &mat7, &mat8, &mat9, &mat10, &mat11, &mat12, &mat13, &mat14, &mat15, &mat16) == 16 || sscanf_s(s.c_str(), "mat4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &mat1, &mat2, &mat3, &mat4, &mat5, &mat6, &mat7, &mat8, &mat9, &mat10, &mat11, &mat12, &mat13, &mat14, &mat15, &mat16) == 16) {
						// Parse the string
						glm::mat4 mat(mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10, mat11, mat12, mat13, mat14, mat15, mat16);
						Push(mat);
					}
					else { throw std::runtime_error("Invalid string entered"); }
					break;
				}
				case Fiea::GameEngine::Datum::Table:
					break;
				case Fiea::GameEngine::Datum::Pointer:
					break;
				default:
					break;
				}
			}
		}
	}

	/**
	 * @brief GetAsString returns the item at index and converts it to string then returns it
	 * @param idx 
	 * @return string representation of item at index
	*/
	std::string Datum::GetAsString(size_t idx) {
		// Throw exception if idx is out of Datum's range
		if (idx >= _DatumSize) 
			throw std::out_of_range("idx is out of range");
		else {
			switch (_type)
			{
			case Fiea::GameEngine::Datum::Int: {
				int ivalue = Get<int>(idx);
				return std::to_string(ivalue);
			}
			case Fiea::GameEngine::Datum::Float: {
				float fvalue = Get<float>(idx);
				return std::to_string(fvalue);
			}
			case Fiea::GameEngine::Datum::String: {
				return Get<std::string>(idx);
			}
			case Fiea::GameEngine::Datum::Vector: {
				glm::vec4 v = Get<glm::vec4>(idx);
				return glm::to_string(v);
			}
			case Fiea::GameEngine::Datum::Matrix: {
				glm::mat4 m = Get<glm::mat4>(idx);
				return glm::to_string(m);
			}
			default:
				// This line will never be reached
				throw std::runtime_error("_type is Unkown");
			}
		}
	};

	/**
	 * @brief Clear quite simply, Clears out the Datum without decreasing it's capacity
	*/
	void Datum::Clear() {
		// Checks if there is anything to clear
		if (!Empty()) {
			// Iterate over Datum and destruct/remove any populated items
			switch (_type)
			{
			case Fiea::GameEngine::Datum::Int: {
				// Create int pointer for easier navigation
				int* intPtr = static_cast<int*>(_mData);
				// Set all elements to Null
				for (size_t i = 0; i < _DatumSize; ++i) {
					intPtr[i] = NULL;
				}
				break;
			}
			case Fiea::GameEngine::Datum::Float: {
				// Create float pointer
				float* fPtr = static_cast<float*>(_mData);
				// Set each populated element to null
				for (size_t i = 0; i < _DatumSize; ++i) {
					fPtr[i] = NULL;
				}
				break;
			}
			case Fiea::GameEngine::Datum::String: {
				// Create string pointer
				std::string* strPtr = static_cast<std::string*>(_mData);
				// Destruct strings
				for (size_t i = 0; i < _DatumSize; ++i) {
					(strPtr + i)->~basic_string();
				}
				break;
			}
			case Fiea::GameEngine::Datum::Vector: {
				// Create glm::vec4 pointer
				glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
				// Set populated vectors to NULL
				for (size_t i = 0; i < _DatumSize; ++i) {
					vecPtr[i] = NULLVEC;
				}
				break;
			}
			case Fiea::GameEngine::Datum::Matrix: {
				// Create glm::mat4 pointer
				glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
				// Set populated Matrices to NULL
				for (size_t i = 0; i < _DatumSize; ++i) {
					matPtr[i] = NULLMAT;
				}
				break;
			}
			}
			// Set size to 0
			_DatumSize = 0;
		}
	};


	/**
	 * @brief Resize will adjust the Datum's size and capacity so that the newSize is equal to capacity
	 * @param newSize 
	*/
	void Datum::Resize(size_t newSize) {
		if (newSize < _DatumSize) {
			// Allocate memory equal to newSize
			switch (_type)
			{
			case Fiea::GameEngine::Datum::Int: {
				//Move current data in Datum up to newSize to new _mData
				int* intPtr = static_cast<int*>(_mData);
				int* newData = (int*)malloc(sizeof(int) * newSize);
				for (size_t i = 0; i < newSize; ++i) {
					new(newData + i) int(intPtr[i]);
				}
				// Free Up old data
				free(_mData);
				// Transfer data to a new _mData
				_mData = newData;
				// Set Size to newSize
				_DatumSize = newSize;
				break;
			}
			case Fiea::GameEngine::Datum::Float: {
				//Move current data in Datum up to newSize to new _mData
				float* floatPtr = static_cast<float*>(_mData);
				float* newData = (float*)malloc(typeSizes[_type] * newSize);
				for (size_t i = 0; i < newSize; ++i) {
					new(newData + i) float(floatPtr[i]);
				}
				// Free Up old data
				free(_mData);
				// Transfer data to a new _mData
				_mData = newData;
				// Set Size to newSize
				_DatumSize = newSize;
				break;
			}
			case Fiea::GameEngine::Datum::String: {
				// Create pointers for better navigation
				std::string* strPtr = static_cast<std::string*>(_mData);
				std::string* newData = (std::string*)malloc(typeSizes[_type] * newSize);
				// Copy over data
				for (size_t i = 0; i < newSize; ++i) {
					new(newData + i) std::string(strPtr[i]);
				}
				// Destroy strings in _mData
				for (size_t j = 0; j < _DatumSize; ++j) {
					strPtr[j].~basic_string();
				}
				free(_mData);
				_mData = newData;
				_DatumSize = newSize;
				break;
			}
			case Fiea::GameEngine::Datum::Vector: {
				//Move current data in Datum up to newSize to new _mData
				glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
				glm::vec4* newData = (glm::vec4*)malloc(typeSizes[_type] * newSize);
				for (size_t i = 0; i < newSize; ++i) {
					new(newData + i) glm::vec4(vecPtr[i]);
				}
				// Free Up old data
				free(_mData);
				// Transfer data to a new _mData
				_mData = newData;
				// Set Size to newSize
				_DatumSize = newSize;
				break;
			}
			case Fiea::GameEngine::Datum::Matrix: {
				//Move current data in Datum up to newSize to new _mData
				glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
				glm::mat4* newData = (glm::mat4*)malloc(typeSizes[_type] * newSize);
				for (size_t i = 0; i < newSize; ++i) {
					new(newData + i) glm::mat4(matPtr[i]);
				}
				// Free Up old data
				free(_mData);
				// Transfer data to a new _mData
				_mData = newData;
				// Set Size to newSize
				_DatumSize = newSize;
				break;
			}
			case Fiea::GameEngine::Datum::Table: {
				Scope** scopePtr = static_cast<Scope**>(_mData);
				Scope** newPtr = (Scope**)malloc(typeSizes[_type] * newSize);
				for (size_t i = 0; i < newSize; ++i) {
					new(newPtr + i) Scope*(scopePtr[i]);
				}
				// Free Up old data
				free(_mData);
				// Transfer data to a new _mData
				_mData = newPtr;
				// Set Size to newSize
				_DatumSize = newSize;
				break;
			}
			default:
				throw std::runtime_error("_type is unsupported");
			}
		}
		else if (newSize > _DatumSize) {
			if(newSize != _DatumCapacity) {
				// Allocate memory equal to newSize
				switch (_type)
				{
				case Fiea::GameEngine::Datum::Int: {
					//Move current data in Datum up to newSize to new _mData
					int* intPtr = static_cast<int*>(_mData);
					int* newData = (int*)malloc(typeSizes[_type] * newSize);
					// Move already populated items
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(newData + i) int(intPtr[i]);
					}
					// Default construct the rest
					for (size_t j = _DatumSize; j < newSize; ++j) {
						newData[j] = NULL;
					}
					// Free Up old data
					free(_mData);
					// Transfer data to a new _mData
					_mData = newData;
					
					break;
				}
				case Fiea::GameEngine::Datum::Float: {
					//Move current data in Datum up to newSize to new _mData
					float* floatPtr = static_cast<float*>(_mData);
					float* newData = (float*)malloc(typeSizes[_type] * newSize);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(newData + i) float(floatPtr[i]);
					}
					// Default construct the rest
					for (size_t j = _DatumSize; j < newSize; ++j) {
						newData[j] = NULL;
					}
					// Free Up old data
					free(_mData);
					// Transfer data to a new _mData
					_mData = newData;
					break;
				}
				case Fiea::GameEngine::Datum::String: {
					// Create pointers for better navigation
					std::string* strPtr = static_cast<std::string*>(_mData);
					std::string* newData = (std::string*)malloc(typeSizes[_type] * newSize);
					// Copy over data
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(newData + i) std::string(strPtr[i]);
					}
					// Destruct strPtr's elements
					for (size_t j = 0; j < _DatumSize; ++j) {
						strPtr[j].~basic_string();
					}
					free(_mData);
					_mData = newData;
					break;
				}
				case Fiea::GameEngine::Datum::Vector: {
					//Move current data in Datum up to newSize to new _mData
					glm::vec4* vecPtr = static_cast<glm::vec4*>(_mData);
					glm::vec4* newData = (glm::vec4*)malloc(typeSizes[_type] * newSize);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(newData + i) glm::vec4(vecPtr[i]);
					}
					// Default construct vectors for the rest
					for (size_t j = _DatumSize; j < newSize; ++j) {
						newData[j] = glm::vec4();
					}
					// Free Up old data
					free(_mData);
					// Transfer data to a new _mData
					_mData = newData;
					break;
				}
				case Fiea::GameEngine::Datum::Matrix: {
					//Move current data in Datum up to newSize to new _mData
					glm::mat4* matPtr = static_cast<glm::mat4*>(_mData);
					glm::mat4* newData = (glm::mat4*)malloc(typeSizes[_type] * newSize);
					for (size_t i = 0; i < _DatumSize; ++i) {
						new(newData + i) glm::mat4(matPtr[i]);
					}
					// Default construct vectors for the rest
					for (size_t j = _DatumSize; j < newSize; ++j) {
						newData[j] = glm::mat4();
					}
					// Free Up old data
					free(_mData);
					// Transfer data to a new _mData
					_mData = newData;
					break;
				}
				case Fiea::GameEngine::Datum::Table: {
					Scope** scopePtr = static_cast<Scope**>(_mData);
					Scope** newPtr = (Scope**)malloc(typeSizes[_type] * newSize);
					for (size_t i = 0; i < newSize; ++i) {
						new(newPtr + i) Scope* (scopePtr[i]);
					}
					// Default construct the Scope*s
					for (size_t j = _DatumSize; j < newSize; ++j) {
						newPtr[j] = nullptr;
					}
					// Free Up old data
					free(_mData);
					// Transfer data to a new _mData
					_mData = newPtr;
					// Set Size to newSize
					_DatumSize = newSize;
					break;
				}
				default:
					throw std::runtime_error("_type is unsupported");
				}
			}
		}
		if (_type == Unknown) {
			throw std::runtime_error("Can't Resize empty Datum");
		}
		_DatumCapacity = newSize;
	};

	const std::string Datum::ToString() const{
		std::stringstream ss;
		ss << GetType();
		ss << "   ";
		ss << Size();
		ss << "    ";
		ss << Capacity();
		return ss.str();
	}

	void Datum::RemoveAt(size_t idx) {
		if (idx >= _DatumSize) {
			throw std::out_of_range("idx is larger than Datum size");
		}
		else {
			switch (_type)
			{
			case Fiea::GameEngine::Datum::Unknown:
				throw std::invalid_argument("Cannot remove from uninitialized Datum");
			case Fiea::GameEngine::Datum::Int:
			{
				int* ptr = static_cast<int*>(_mData);
				ptr[idx] = NULL;
				for (size_t i = idx; i < _DatumSize - 1; ++i) {
					ptr[i] = ptr[i + 1];
				}
				--_DatumSize;
				break;
			}
			case Fiea::GameEngine::Datum::Float:
			{
				float* ptr = static_cast<float*>(_mData);
				ptr[idx] = NULL;
				for (size_t i = idx; i < _DatumSize - 1; ++i) {
					ptr[i] = ptr[i + 1];
				}
				--_DatumSize;
				break;
			}
			case Fiea::GameEngine::Datum::String:
			{
				std::string* ptr = static_cast<std::string*>(_mData);
				ptr[idx] = std::string();
				for (size_t i = idx; i < _DatumSize - 1; ++i) {
					ptr[i] = ptr[i + 1];
				}
				--_DatumSize;
				break;
			}
			case Fiea::GameEngine::Datum::Vector:
			{
				glm::vec4* ptr = static_cast<glm::vec4*>(_mData);
				ptr[idx] = glm::vec4();
				for (size_t i = idx; i < _DatumSize - 1; ++i) {
					ptr[i] = ptr[i + 1];
				}
				--_DatumSize;
				break;
			}
			case Fiea::GameEngine::Datum::Matrix:
			{
				glm::mat4* ptr = static_cast<glm::mat4*>(_mData);
				ptr[idx] = glm::mat4();
				for (size_t i = idx; i < _DatumSize - 1; ++i) {
					ptr[i] = ptr[i + 1];
				}
				--_DatumSize;
				break;
			}
			case Fiea::GameEngine::Datum::Table:
			{
				Scope** ptr = static_cast<Scope**>(_mData);
				ptr[idx] = nullptr;
				if ((idx + 1) < _DatumSize) {
					for (size_t i = idx; i < _DatumSize - 1; ++i) {
						ptr[i] = ptr[i + 1];
					}
					ptr[_DatumSize - 1] = nullptr;
				}
				--_DatumSize;
				break;
			}
			case Fiea::GameEngine::Datum::Pointer:
			{
				RTTI** ptr = static_cast<RTTI**>(_mData);
				ptr[idx] = nullptr;
				for (size_t i = idx; i < _DatumSize - 1; ++i) {
					ptr[i] = ptr[i + 1];
				}
				--_DatumSize;
				break;
			}
			default:
				throw std::invalid_argument("Type unsupported");
			}
		}
	};
};
