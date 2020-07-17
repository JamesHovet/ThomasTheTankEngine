//
//  EntityAdmin.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"

#include <stdlib.h>

#include "AllComponents.h"
#include "DebugPrintSystem.hpp"
#include "object_pool.hpp"

// Helpers:
void constructComponentPools(std::array<void *, NUM_COMPONENTS>& pool,
                             std::array<std::function<void (void *)>, NUM_COMPONENTS>& destroyers,
                             std::vector<std::function<void (void)>>& cleanup_callbacks){
    
    { // Transform is built in.
        boost::object_pool<TransformComponent>* p = new boost::object_pool<TransformComponent>;
        pool[0] = p;
        destroyers[0] = [p](void * ptr){
            p->free((boost::object_pool<TransformComponent>::element_type *) ptr);
        };
        cleanup_callbacks.push_back([p](void) { delete p; });
    }
    { // DebugName is built in.
        boost::object_pool<DebugNameComponent>* p = new boost::object_pool<DebugNameComponent>;
        pool[1] = p;
        destroyers[1] = [p](void * ptr){
            p->free((boost::object_pool<DebugNameComponent>::element_type *) ptr);
        };
        cleanup_callbacks.push_back([p](void) { delete p; });
    }
    
    #include "populatePoolsInclude.cpp"
//    boost::object_pool<DebugInfoComponent>* p = new boost::object_pool<DebugInfoComponent>;
//    std::function<void (void *)> destroyer = [p](void * ptr) {
//        p->free((boost::object_pool<DebugInfoComponent>::element_type *) ptr);
//    };
//    std::function<void (void)> cleanup = [p](void) {
//        delete p;
//    };
//
//    pool[0] = p;
//    destroyers[0] = destroyer;
//    cleanup_callbacks.push_back(cleanup);
//
    
}

// EntityAdmin member functions
EntityAdmin::EntityAdmin(){
    m_systems.reserve(Systems::SystemsCount);
    constructComponentPools(m_components_pool_array,
                            m_components_destuction_callbacks_array,
                            m_cleanup_callbacks);
}


EntityAdmin::~EntityAdmin(){
    for (System * s : m_systems){
        delete s;
    }
    // cleanup callbacks including calling destructors on all the component object pools
    for (auto cleanup_callback : m_cleanup_callbacks){
        std::invoke(cleanup_callback);
    }
}

entityID EntityAdmin::createEntity(){
    m_entities_dirty = true;
    assert(m_entities.size() < MAX_ENTITIES);
    
    // pseudorandom entity ID
    entityID eID = rand() % MAX_ENTITIES;
    while(m_entities.count(eID) != 0){
        eID = rand() % MAX_ENTITIES;
    }
    
    Entity* e = m_entity_pool.construct(eID);
    m_entities[eID] = e;
    m_component_maps.insert(std::make_pair(eID, std::unordered_map<componentID, Component*>()));
    
    return eID;
}

void EntityAdmin::destroyEntity(entityID eID){
    m_entities_dirty = true;
    for(auto pair : m_component_maps.at(eID)){
        auto componentID = pair.first;
        auto componentPtr = pair.second;
        std::invoke(m_components_destuction_callbacks_array[componentID], componentPtr);
    }
    Entity* e = m_entities[eID];
    m_entity_pool.destroy(e);
}

void EntityAdmin::setup(){
    m_systems.push_back(new DebugPrintSystem(*this));

    //@Remove: temporary test entities
    for(int i = 0; i < 1024; i++){
        entityID eID = this->createEntity();
        DebugNameComponent& nameC = this->addComponent<DebugNameComponent>(eID);
        TransformComponent& transformC = this->addComponent<TransformComponent>(eID);

        nameC.m_name = std::to_string(eID);
        transformC.m_position = glm::vec3((float) i, 0, (float) eID);
        
        m_systems[Systems::DebugPrintSystem]->registerEntity(eID);
    }
    
    return;
    
}

void EntityAdmin::update(float dt){
    for (System* s : m_systems){
        s->tick(1.0);
    }
}

void EntityAdmin::teardown(){
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it){
        destroyEntity(it->first);
    }
    
//    for(auto it = m_entities.begin(); it != m_entities.end(); ++it){
//        destr
//    }
}

void EntityAdmin::mainLoop(){
    
}
