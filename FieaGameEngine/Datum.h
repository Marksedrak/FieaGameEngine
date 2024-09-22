#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "RTTI.h"

namespace Fiea::GameEngine {
	class Scope;

	class Datum {
	friend Scope;
	public:

		// Enum determining Data Type
		enum DatumType {
			Unknown,
			Int,
			Float,
			String,
			Vector,
			Matrix,
			Table,
			Pointer
		};

		// Default Constructor -------------------------------------------------------------------------------------------
		Datum();

		// Parameterized Constructors ------------------------------------------------------------------------------------
		template<class T>
		Datum(T value);

		template<>
		Datum(const char* value);

		template<class T>
		Datum(T value, size_t size);

		template<>
		Datum(const char* value, size_t size);

		// Copy Constructor -----------------------------------------------------------------------------------------------

		Datum(Datum& other);
		Datum(const Datum& other);

		// Move Constructor -----------------------------------------------------------------------------------------------

		Datum(Datum&& other) noexcept;

		// Assignment Operator -------------------------------------------------------------------------------------------------

		void operator=(const Datum& other);

		// Assignment Operator Overloads
		void operator=(int i);
		void operator=(float f);
		void operator=(std::string s);
		void operator=(glm::vec4 v);
		void operator=(glm::mat4 m);
		void operator=(Datum&& other) noexcept;

		bool operator==(Datum& rhs);
		bool operator==(const Datum& rhs) const;
		bool operator!=(Datum& rhs);
		bool operator!=(const Datum& rhs) const;

		// Destructor
		~Datum();

		template<class T>
		void Push(T value);

		template<>
		void Push(const char* value);

		void Pop();

		// Set
		template<class T>
		void Set(size_t idx, T& valueRef);

		void SetFromString(size_t idx, std::string value);
		

		// Retrieving Methods (GET)

		template<typename T>
		T& Get(size_t idx= 0);

		template<typename T>
		const T& Get(size_t idx= 0) const;

		int& GetInt(size_t idx = 0);
		const int& GetInt(size_t idx = 0) const;
		float& GetFloat(size_t idx = 0);
		const float& GetFloat(size_t idx = 0) const;
		std::string& GetString(size_t idx = 0);
		const std::string& GetString(size_t idx = 0) const;
		glm::vec4& GetVector(size_t idx = 0);
		const glm::vec4& GetVector(size_t idx = 0) const;
		glm::mat4& GetMatrix(size_t idx = 0);
		const glm::mat4& GetMatrix(size_t idx = 0) const;
		Scope* GetScope(size_t idx = 0);
		const Scope* GetScope(size_t idx = 0) const;
		RTTI* GetPointer(size_t idx = 0);
		const RTTI* GetPointer(size_t idx = 0) const;

		// Retrieval as String
		std::string GetAsString(size_t idx = 0);
		
		// Type
		const std::string GetType() const;

		template<class T>
		void SetType(T value);

		void SetTypeByType(DatumType type);

		template<class T>
		bool CheckType(T value);

		bool CheckType(DatumType type);

		// Size
		void Resize(size_t newSize);
		void Clear();
		bool Empty();
		size_t Size() { return _DatumSize; };
		const size_t Size() const { return _DatumSize; };
		size_t Capacity() { return _DatumCapacity; };
		const size_t Capacity() const { return _DatumCapacity; };

		template<class T>
		void SetStorage(T* array, int elementNum);
		
		template<class T>
		void SetStorage(T* array, int elementNum, DatumType type);

		// no implementation in headers for now... move to inl or cpp

		// Scope
		void RemoveAt(size_t idx);



		// operator[]
		Scope& operator[](std::uint32_t idx) { return *GetScope(idx); }

		const std::string ToString() const;

	private:

		size_t typeSizes[8] = {
			sizeof(void*),
			sizeof(int),
			sizeof(float),
			sizeof(std::string),
			sizeof(glm::vec4),
			sizeof(glm::mat4x4),
			sizeof(Scope*),
			sizeof(RTTI*)
		};

		DatumType _type = Unknown; // an Enum determining the type of elements inside the Datum
		size_t _DatumSize = 0; //Datum's size
		size_t _DatumCapacity = 0; //Datum's Capacity
		void* _mData = nullptr; //pointer to the first element in the Datum
		bool externalStorage = false;
	};
}

#include "Datum.inl"