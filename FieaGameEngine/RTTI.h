#pragma once

#include <cstddef>
#include <string>

namespace Fiea::GameEngine
{
	class RTTI
	{
	public:
		using IdType = std::size_t;

		RTTI() = default;
		RTTI(const RTTI&) = default;
		RTTI& operator=(const RTTI&) = default;
		RTTI(RTTI&&) noexcept = default;
		RTTI& operator=(RTTI&&) noexcept = default;
		virtual ~RTTI() = default;

		virtual IdType TypeIdInstance() const = 0;
		virtual bool Is(IdType) const;

		template <typename T>
		T* As();

		template <typename T>
		const T* As() const;

		virtual std::string ToString() const;
		virtual bool Equals(const RTTI* rhs) const;
	};
}

#define RTTI_DECLARATIONS(Type, ParentType)																							\
	public:																													\
		static std::string TypeName() { return std::string(#Type); }														\
		static Fiea::GameEngine::RTTI::IdType TypeIdClass() { return _typeId; }												\
		Fiea::GameEngine::RTTI::IdType TypeIdInstance() const override { return TypeIdClass(); }								\
		bool Is(Fiea::GameEngine::RTTI::IdType id) const override { return (id == _typeId ? true : ParentType::Is(id)); }		\
	private:																												\
		static const Fiea::GameEngine::RTTI::IdType _typeId

#define RTTI_DEFINITIONS(Type)																								\
	const Fiea::GameEngine::RTTI::IdType Type::_typeId = reinterpret_cast<Fiea::GameEngine::RTTI::IdType>(&Type::_typeId)
	

#include "RTTI.inl"