//
//  EntityAdmin.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"
#include "AllComponents.hpp"
#include "DebugPrintSystem.hpp"
#include "object_pool.hpp"


void constructComponentPools(std::array<void *, NUM_COMPONENTS>& pool, std::array<std::function<void (void *)>, NUM_COMPONENTS>& destroyers);

EntityAdmin::EntityAdmin(){
    m_systems.reserve(Systems::SystemsCount);

    constructComponentPools(m_components_pool_array, m_components_destroyers_array);
    
}

void constructComponentPools(std::array<void *, NUM_COMPONENTS>& pool, std::array<std::function<void (void *)>, NUM_COMPONENTS>& destroyers){
//    pool[Components::DebugInfoComponent] = new std::vector<DebugInfoComponent>();
    //#include "populatePoolsInclude.cpp"
    boost::object_pool<DebugInfoComponent>* p0 = new boost::object_pool<DebugInfoComponent>;
    std::function<void (void *)> destroyer0 = [p0](void * ptr) {
        p0->free((boost::object_pool<DebugInfoComponent>::element_type *) ptr);
        std::cout << "destroyed " << ptr << std::endl;
    };
    pool[0] = p0;
    destroyers[0] = destroyer0;


}

EntityAdmin::~EntityAdmin(){
    
}

entityID EntityAdmin::createEntity(){
    entityID id = this->nextFreeID++;
    assert(id < MAX_ENTITIES);
    Entity* e = m_entity_pool.construct(id);
    m_entities[id] = e;
    return id;
}

void EntityAdmin::destroyEntity(entityID eID){
    Entity* e = m_entities[eID];
    for(auto pair : e->m_componentMap){
        int componentID = pair.first;
        void * pool = m_components_pool_array[0];

        m_components_destroyers_array[0](pair.second);
        
    }
    m_entity_pool.free(e);
}

void EntityAdmin::setup(){
    m_systems.push_back(new DebugPrintSystem(*this));
    

    for(int i = 0; i < 1024; i++){
        entityID eID = this->createEntity();
        DebugInfoComponent& c = this->addComponent<DebugInfoComponent>(eID);

        c.str = std::to_string(eID);
        m_systems[Systems::DebugPrintSystem]->registerEntity(eID);
    }
    
    return;
    
}

void EntityAdmin::update(float dt){
    for (System* s : m_systems){
        s->tick(1.0);
    }
    for(int i = 0; i < 1024; i ++){
        destroyEntity(i);
    }
}

void EntityAdmin::mainLoop(){
    
}
