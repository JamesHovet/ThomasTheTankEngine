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

void constructComponentPools(std::array<void *, NUM_COMPONENTS>& pool);

EntityAdmin::EntityAdmin(){
    m_systems.reserve(Systems::SystemsCount);

    constructComponentPools(m_components_pool_array);
    
}

void constructComponentPools(std::array<void *, NUM_COMPONENTS>& pool){
//    pool[Components::DebugInfoComponent] = new std::vector<DebugInfoComponent>();
    //#include "populatePoolsInclude.cpp"
    pool[0] = new boost::object_pool<DebugInfoComponent>;
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

void EntityAdmin::destroyEntity(entityID e){
    //TODO
}

void EntityAdmin::setup(){
    m_systems.push_back(new DebugPrintSystem(*this));
    

    for(int i = 0; i < 1024; i++){
        entityID eID = this->createEntity();
        DebugInfoComponent& c = this->addComponent<DebugInfoComponent>(eID);

        c.buf[0] = 48 + eID;
        m_systems[Systems::DebugPrintSystem]->registerEntity(eID);
    }
    
    return;
    
}

void EntityAdmin::update(float dt){
    for (System* s : m_systems){
        s->tick(1.0);
    }
}

void EntityAdmin::mainLoop(){
    
}
