//
//  EntityAdmin.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef EntityAdmin_hpp
#define EntityAdmin_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include "object_pool.hpp"
#include <functional>

#include "Component.hpp"
#include "Entity.hpp"
//#include "System.hpp"
#include "SystemsEnum.hpp"
#include "ComponentsEnum.hpp"
#include "AllComponents.hpp"
#include "SingletonsEnum.hpp"


#define MAX_ENTITIES 2048

class System;

class EntityAdmin {
public:
    EntityAdmin();
    ~EntityAdmin();
    
    void setup();
    void update(float dt);
    void mainLoop(void);
    
    entityID createEntity();
    void destroyEntity(entityID e);
//    template <typename T>
//    T* addComponent(entityID e, Components type){
//        boost::object_pool<T>* this_pool = ((boost::object_pool<T>*)m_components_pool_array.at((int)type));
//        T* out = this_pool->construct();
//        return out;
//    }
    template <typename T>
    T& addComponent(entityID e){
        int componentID = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        boost::object_pool<T>* this_pool = ((boost::object_pool<T>*)m_components_pool_array.at(componentID));
        T* out = this_pool->construct();
        m_entities.at(e)->m_componentMap.insert(std::make_pair(componentID, out));
        return *out;
    }
    
    template <typename T>
    T& getComponent(entityID eID){
        Entity* e = m_entities.at(eID);
        return e->getComponent<T>();
    }

//private:
    
    std::unordered_map<entityID, Entity*> m_entities;
    std::vector<System *> m_systems;
private:
    std::array<void *, NUM_COMPONENTS> m_components_pool_array;
    std::array<std::function<void (void *)>, NUM_COMPONENTS> m_components_destroyers_array;
    entityID nextFreeID = 0;
    boost::object_pool<Entity> m_entity_pool;
    std::array<void *, Singletons::SingletonsCount> m_singletons;
};

#endif /* EntityAdmin_hpp */
