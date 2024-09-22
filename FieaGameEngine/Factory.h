#pragma once

#include <string>
#include <unordered_map>
#include <memory>


namespace Fiea::GameEngine {
	
	template<class BaseClass>
	class IFactory
	{
	public:
		// Return the address of a concrete product associated with this concrete factory class.
		// Hint : what qualifier should this routine have ?
		[[nodiscard]] virtual BaseClass* Create() const = 0;

		// Return a string representing the name of the class the factory instantiates.
		virtual const std::string ClassName() const = 0;
	};

	template<class BaseClass>
	class FactoryManager
	{
	public:
		// Given a class name (string), return the associated concrete factory. This should run in constant time.
		static const IFactory<BaseClass>& Find(const std::string& ClassName);

		// Given a class name (string), return a new object of that type. 
		// This should run in constant time (with respect to name lookup – not associated constructor costs).
		static BaseClass* Create(const std::string& ClassName);


		// Given a reference to a concrete factory, add it to the list of factories for this abstract factory.
		// (Question: should this be public?)
		static void Add(std::unique_ptr<IFactory<BaseClass>>&& Fptr, const std::string& key);


		// Given a reference to a concrete factory, remove it from the list of factories for this abstract factory.
		// (Question: should this be public?)
		static void Remove(const std::string& key);

		static void Clear();

	private:
		inline static std::unordered_map<std::string, std::unique_ptr<IFactory<BaseClass>>> FactoryMap;
	};
}

#include "FactoryManager.inl"

// ConcreteFactory macro
// This should perform everything needed to create a concrete factory for a given product, such that the only code you need to implement a factory for a given class is this:
// ConcreteFactory(ConcreteProductType, AbstractProductType)

#define ConcreteFactory(_Base, _Concrete)																				\
		class _Concrete##Factory final : public IFactory<_Base> {														\
			public:																										\
			[[nodiscard]] _Concrete* Create() const override{																					\
				return new _Concrete();																					\
			}																											\
																														\
			const std::string ClassName() const override{																		\
				return #_Concrete;																						\
			}																											\
		};																												\
																														\
		std::unique_ptr<_Concrete##Factory> _Concrete##factPtr = std::make_unique<_Concrete##Factory>();							\
		FactoryManager<_Base>::Add(std::move(_Concrete##factPtr), #_Concrete);