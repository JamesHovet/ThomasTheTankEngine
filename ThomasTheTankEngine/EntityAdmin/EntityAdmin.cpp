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
#include "allFamilies.h"
#include "object_pool.hpp"
#include "ECSUtils.hpp"

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
    
    #include "populateComponentPoolsInclude.cpp"
}

//void constructFamilyVectors(std::array<void *, NUM_FAMILIES>& array, std::vector<std::function<void (void)>>& cleanup_callbacks){
//    #include "populateFamilyVectorsInclude.cpp"
////    {
////        std::vector<GreyBoxFamily>* v = new std::vector<GreyBoxFamily>;
////        array[0] = v;
////        cleanup_callbacks.push_back([v](void) {delete v;});
////    }
//}

void constructFamilyMaps(std::array<void *, NUM_FAMILIES>& array, std::vector<std::function<void (void)>>& cleanup_callbacks){
    #include "populateFamilyMapsInclude.cpp"
//    {
//        std::unordered_map<entityID, GreyBoxFamily>* m = new std::unordered_map<entityID, GreyBoxFamily>;
//        array[0] = m;
//        cleanup_callbacks.push_back([m](void) {delete m;});
//    }
}
    
// EntityAdmin member functions
EntityAdmin::EntityAdmin()
    : m_DebugPrintSystem(*this),
    m_GreyBoxRenderSystem(*this)
{
    constructComponentPools(m_components_pool_array,
                            m_components_destuction_callbacks_array,
                            m_cleanup_callbacks);
    
    constructFamilyMaps(m_families_maps_array, m_cleanup_callbacks);
}


EntityAdmin::~EntityAdmin(){
//    for (System * s : m_systems){
//        delete s;
//    }
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
    
    m_DebugPrintSystem.init();
    m_GreyBoxRenderSystem.init();
    
    //@Remove: temporary test entities
    for(int i = 0; i < 5; i++){
        entityID eID = this->createEntity();
        DebugNameComponent& nameC = this->addComponent<DebugNameComponent>(eID);
        TransformComponent& transformC = this->addComponent<TransformComponent>(eID);
        GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(eID);

        nameC.m_name = std::to_string(eID);
        transformC.m_position = glm::vec3(((float) i - 2) / 4.0f);
        boxC.m_color = glm::vec3(((float) i) * 0.2, 0.0, 0.0);
    }
    
    return;
    
}

void EntityAdmin::update(float dt){
    if(m_entities_dirty){
        filterEntitiesIntoFamilies();
    }
    
//    m_DebugPrintSystem.tick(dt);
}

void EntityAdmin::render(){
    m_GreyBoxRenderSystem.render();
}

void EntityAdmin::teardown(){
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it){
        destroyEntity(it->first);
    }
}

void EntityAdmin::mainLoop(){
    
}

void EntityAdmin::clearFamilies(){
    #include "clearFamiliesInclude.cpp"
//    getFamilyMap<DebugPrintableFamily>().clear();
}

void EntityAdmin::filterEntitiesIntoFamilies(){
    this->clearFamilies();
    
    for (std::pair<entityID, Entity*> pair : m_entities){
        Entity* e = pair.second;
        componentMask mask = e->m_mask;
        entityID eID = e->m_entityID;
        
        #include "filterEntitiesIntoFamiliesInclude.cpp"
        
//        if(ECSUtils::doesPassFilter(mask, Family<DebugPrintableFamily>::mask)){
//            DebugPrintableFamily family = DebugPrintableFamily(eID, getComponent<DebugPrintComponent>(eID));
//            getFamilyMap<DebugPrintableFamily>().emplace(std::make_pair(eID, family));
//        }
    }
}
