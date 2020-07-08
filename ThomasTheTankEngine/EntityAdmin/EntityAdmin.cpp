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

void populatePool(std::array<void *, Components::ComponentsCount>& pool);

EntityAdmin::EntityAdmin(){
    m_systems.reserve(Systems::SystemsCount);
    m_entity_pool.reserve(MAX_ENTITIES);
    
    populatePool(m_components_pool_array);
    
}

void populatePool(std::array<void *, Components::ComponentsCount>& pool){
    pool[Components::DebugInfoComponent] = new std::vector<DebugInfoComponent>();
}

EntityAdmin::~EntityAdmin(){
    
}

entityID EntityAdmin::createEntity(){
    entityID id = m_entity_pool.size();
    assert(id < MAX_ENTITIES);
    m_entity_pool.push_back(Entity(id));
    m_entities[id] = &m_entity_pool.at(id);
    return id;
}

void EntityAdmin::destroyEntity(entityID e){
    //TODO
}

void EntityAdmin::setup(){
    m_systems.push_back(new DebugPrintSystem(*this));
    

    for(int i = 0; i < 1024; i++){
        entityID eID = this->createEntity();
        this->addComponent<DebugInfoComponent>(eID, Components::DebugInfoComponent);
        DebugInfoComponent* c = this->getComponent<DebugInfoComponent>(eID, Components::DebugInfoComponent);
        
        c->buf[0] = 48 + eID;
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
