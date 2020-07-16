//
//  Entity.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <algorithm>
#include "typedefs.h"
#include "Component.hpp"
#include <map>
class EntityAdmin;

struct Entity {
    friend EntityAdmin;
private:
    std::map<int, Component*> m_componentMap;
public:
    entityID m_entityID;

    Entity(entityID eID){
        m_entityID = eID;
    }
    
private:
    template <typename T>
    T* tryGetComponent(){
        constexpr int id = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        if (m_componentMap.count(id) != 0){
            return static_cast<T*>(m_componentMap.at(id));
        }
        return nullptr;
    }
    
    template <typename T>
    T& getComponent(){
        auto value = tryGetComponent<T>();
        assert(value != nullptr);
        return *value;
    }
    
    template <typename T>
    const T& getComponent() const {
        auto value = tryGetComponent<T>();
        assert(value != nullptr);
        return *value;
    }
    
};

#endif /* Entity_hpp */
