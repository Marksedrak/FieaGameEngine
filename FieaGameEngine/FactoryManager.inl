#pragma once
#include "Factory.h"

namespace Fiea::GameEngine {
	
	/** Find
	 * @brief Gets the Factory related to the class
	 * @tparam BaseClass : Base class of Factory
	 * @param ClassName : name of the class to return the factory of
	 * @return returns a reference of the factory related to the class
	*/
	template<class BaseClass>
	const IFactory<BaseClass>& FactoryManager<BaseClass>::Find(const std::string& ClassName)
	{
		auto pair =  FactoryMap.find(ClassName);
		if (pair->second != nullptr) {
			return *pair->second;
		}
		else {
			throw std::invalid_argument("ClassName is invalid");
		}
	}

	/** Create
	 * @brief Creates a new instance of the class
	 * @tparam BaseClass : Base of class
	 * @param ClassName : Name of class
	 * @return Pointer to newly created instance of class, nullptr otherwise
	*/
	template<class BaseClass>
	BaseClass* FactoryManager<BaseClass>::Create(const std::string& ClassName)
	{
		return Find(ClassName).Create();
	}

	/** Add
	 * @brief Adds Factory to map of Factories with a corresponding key
	 * @tparam BaseClass : Base class of current class
	 * @param Fptr : unique_ptr of Factory
	 * @param key : string with key to use for the map
	*/
	template<class BaseClass>
	void FactoryManager<BaseClass>::Add(std::unique_ptr<IFactory<BaseClass>>&& Fptr, const std::string& key)
	{
		if (FactoryMap.find(key) == FactoryMap.end()) {
			FactoryMap[key] = std::move(Fptr);
		}
		else {
			throw std::logic_error("Adding duplicate factories");
		}
	}

	/** Remove
	 * @brief Removes Factory from list
	 * @tparam BaseClass : base class
	 * @param key : key associated to Factory in list wanting to remove
	*/
	template<class BaseClass>
	void FactoryManager<BaseClass>::Remove(const std::string& key)
	{
		FactoryMap.erase(key);
	}


	template<class BaseClass>
	void FactoryManager<BaseClass>::Clear() 
	{
		FactoryMap.clear();
	}

}