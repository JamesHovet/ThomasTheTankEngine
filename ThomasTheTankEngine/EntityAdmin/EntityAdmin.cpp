//
//  EntityAdmin.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"
#include "DebugPrintSystem.hpp"

void populatePool(std::array<void *, Components::ComponentsCount>&);

EntityAdmin::EntityAdmin(){
    m_systems.reserve(Systems::SystemsCount);
    
    populatePool(m_components_pool);
    
}

void populatePool(std::array<void *, Components::ComponentsCount>& pool){
    pool[Components::DebugInfoComponent] = new std::vector<DebugInfoComponent>();
}

EntityAdmin::~EntityAdmin(){
    
}

void EntityAdmin::setup(){
    m_systems.push_back(new DebugPrintSystem(this));
    
    Entity* e = new Entity(0);
    m_entities[0] = e;
    
    auto tmp = (std::vector<DebugInfoComponent>*) m_components_pool.at(Components::DebugInfoComponent);
    tmp->emplace_back();
    tmp->back().buf[0] = 'a';
    tmp->back().buf[1] = 'b';
    tmp->back().buf[2] = 'c';
    e->m_components[Components::DebugInfoComponent] = tmp->size() - 1;
    
    
    m_systems[Systems::DebugPrintSystem]->registerEntity(0);
    
//    e->m_components[Components::DebugInfoComponent] =  
//
//    m_entities.emplace(std::make_pair(0,new Entity(0)));
//    m_entities.emplace(std::make_pair(1,new Entity(1)));
//    m_systems[Systems::DebugPrint]->registerEntity(0);
//    m_systems[Systems::DebugPrint]->registerEntity(1);
//
//    m_systems[Systems::DebugPrint]->unregisterEntity(0);
//    delete m_entities.at(0);
//    m_entities.erase(0);
    
}

void EntityAdmin::update(float dt){
    for (System* s : m_systems){
        s->tick(1.0);
    }
}

void EntityAdmin::mainLoop(){
    
}
