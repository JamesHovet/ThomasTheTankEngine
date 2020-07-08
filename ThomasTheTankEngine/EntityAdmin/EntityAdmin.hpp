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
#include <array>

#include "Component.hpp"
#include "Entity.hpp"
//#include "System.hpp"
#include "SystemsEnum.hpp"
#include "ComponentsEnum.hpp"
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
    template <typename T>
    void addComponent(entityID e, Components::ComponentType type){
        auto this_pool = ((std::vector<T>*)m_components_pool_array.at(type));
        this_pool->emplace_back();
        m_entities.at(e)->m_components[type] = this_pool->size() - 1;
        T* component = &this_pool->back();
        ((Component*) component)->m_parentEntity = m_entities.at(e);
    }

    template <typename T>
    T* getComponent(entityID e, Components::ComponentType type){
        auto idx = m_entities.at(e)->m_components[type];
        std::vector<T>* componentArray = (std::vector<T>*)m_components_pool_array.at(type);
        return &(componentArray->at(idx));
    }
    
//private:
    
    std::map<entityID, Entity*> m_entities;
    std::vector<System *> m_systems;
private:
    std::array<void *, Components::ComponentsCount> m_components_pool_array;
    std::vector<Entity> m_entity_pool;
    std::array<void *, Singletons::SingletonsCount> m_singletons;
    
    //TODO: Replace the entity pool vector with an actual object pool data structure
    //TODO: Same thing for the components pool, probably using a block allocator
    
};

#endif /* EntityAdmin_hpp */
