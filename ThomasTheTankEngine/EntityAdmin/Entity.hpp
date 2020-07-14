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

struct Entity {
    std::map<int, Component*> componentMap;
    entityID m_entityID;

    Entity(entityID eID){
        m_entityID = eID;
    }
    template <typename T>
    T* tryGetComponent(){
        constexpr int id = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        if (componentMap.count(id) != 0){
            return static_cast<T*>(componentMap.at(id));
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
    
    /* from Halley
     template <typename T>
     bool hasComponent(World& world) const
     {
         if (dirty) {
             return tryGetComponent<T>() != nullptr;
         } else {
             return hasBit(world, FamilyMask::RetrieveComponentIndex<T>::componentIndex);
         }
     }
     */
};

#endif /* Entity_hpp */
