#pragma once
#include <unordered_map>
#include <vector>
#include "Signature.h"

class TypeManager{
    public:
        static const std::vector<Fiea::GameEngine::Signature>& get(size_t typeID)
        {
            assert(_map != nullptr);
            return _map->at(typeID);
        }

        static void add(size_t typeID, const std::vector<Fiea::GameEngine::Signature>& s)
        {
            //lazy initialization
            if (_map == nullptr) _map = new std::unordered_map<size_t, std::vector<Fiea::GameEngine::Signature>>();

            auto ret = _map->insert(std::make_pair(typeID, s));

            assert(ret.second); // avoid double registration
        }

        static void Clear() {
            if (_map != nullptr) {
                _map->clear();
            }
        }

    private:
        inline static std::unordered_map<size_t, std::vector<Fiea::GameEngine::Signature>>* _map;
};
