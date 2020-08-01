//
//  EntityAdmin.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"

#include <stdlib.h>
#include <fstream>
#include <ostream>
#include <istream>
#include <iostream>

#include "json.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include "Component.hpp"
#include "AllComponents.h"
#include "allFamilies.h"
#include "object_pool.hpp"
#include "ECSUtils.hpp"

#include "componentStringSerializationMaps.hpp"
const std::unordered_map<int, std::string> ComponentIDToStringStruct::map = ComponentIDToStringStruct::create_map();

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
    {
        boost::object_pool<CameraComponent>* p = new boost::object_pool<CameraComponent>;
        pool[2] = p;
        destroyers[2] = [p](void * ptr){
            p->free((boost::object_pool<CameraComponent>::element_type *) ptr);
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

void constructFamilyStaticVectors (std::array<void *, NUM_FAMILIES>& array,std::vector<std::function<void (void)>>& cleanup_callbacks){
    {
        std::vector<CameraFamilyStatic>* v = new std::vector<CameraFamilyStatic>;
        array[0] = v;
        cleanup_callbacks.push_back([v](void) {delete v;});
        Family<CameraFamilyStatic>::mask.set(0);
        Family<CameraFamilyStatic>::mask.set(2);
    }
    
    #include "populateFamilyStaticVectorsInclude.cpp"
}

void constructFamilyMaps(std::array<void *, NUM_FAMILIES>& array, std::vector<std::function<void (void)>>& cleanup_callbacks){
    
    {
        std::unordered_map<entityID, CameraFamily>* m = new std::unordered_map<entityID, CameraFamily>;
        array[0] = m;
        cleanup_callbacks.push_back([m](void) {delete m;});
        Family<CameraFamily>::mask.set(0);
        Family<CameraFamily>::mask.set(2);
    }

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
    m_GreyBoxRenderSystem(*this),
    m_EditorSystem(*this),
    m_InputSystem(*this),
    m_ConsoleSystem(*this)
{
    constructComponentPools(m_components_pool_array,
                            m_components_destuction_callbacks_array,
                            m_cleanup_callbacks);
    
    constructFamilyMaps(m_families_maps_array, m_cleanup_callbacks);
    constructFamilyStaticVectors(m_families_maps_static_array, m_cleanup_callbacks);
    
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

bool EntityAdmin::createEntity(entityID eID){
    assert(m_entities.size() < MAX_ENTITIES);
    m_entities_dirty = true;
    if(m_entities.count(eID) != 0){
        return false;
    }
    
    Entity* e = m_entity_pool.construct(eID);
    m_entities[eID] = e;
    m_component_maps.insert(std::make_pair(eID, std::unordered_map<componentID, Component*>()));
    
    return true;
}

entityID EntityAdmin::createEntity(){
    assert(m_entities.size() < MAX_ENTITIES);
    m_entities_dirty = true;
    
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

void EntityAdmin::initAllSystems(){
    m_InputSystem.init();
    m_EditorSystem.init();
    m_ConsoleSystem.init();
    m_DebugPrintSystem.init();
    m_GreyBoxRenderSystem.init();

}

void EntityAdmin::setup(){
    initAllSystems();
    
    { // create camera
        //@Remove: temporary test camera
        entityID eID = this->createEntity();
        TransformComponent& transformC = addComponent<TransformComponent>(eID);
        transformC.m_position = glm::vec3(1.0f, 0.0f, -3.0f);
        CameraComponent& cameraC = addComponent<CameraComponent>(eID);
//        cameraC.m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
        nameC.m_name = "Main Camera";
        
    }
    { // create testing boxes
        //@Remove: temporary test entities
        int numToAdd = 5;
        for(int i = 0; i < numToAdd; i++){
            entityID eID = this->createEntity();
//            DebugNameComponent& nameC = this->addComponent<DebugNameComponent>(eID);
            TransformComponent& transformC = this->addComponent<TransformComponent>(eID);
            GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(eID);
            DebugPrintComponent& debugPrintC = this->addComponent<DebugPrintComponent>(eID);
            
//            debugPrintC.m_enabled = false;

//            nameC.m_name = std::to_string(eID);
            transformC.m_position = glm::vec3(((float) i - (numToAdd / 2)) / (float) numToAdd);
            // transformC.m_orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
            boxC.m_color = glm::vec4(((float) i) * (1.0f / (float) numToAdd), 0.0, 0.0, 1.0f);
        }
    }
    
    m_EditorSingleton.shouldUseEditorCamera = true;
    m_EditorSingleton.shouldTakeEditorInput = true;
    m_InputSingleton.shouldSendKeysTo = KEY_INPUT_MODE::EDITOR;
    
    return;
    
}

void EntityAdmin::filterIfNeeded(){
    if(m_entities_dirty){
        this->clearFamilies();
        filterEntitiesIntoMutableFamilies();
        m_entities_dirty = false; 
    }
}

void EntityAdmin::clearStaticFamilyVectors(){
    getFamilyStaticVector<CameraFamilyStatic>().clear();
    #include "clearStaticFamilyVectorsInclude.cpp"
}

void EntityAdmin::copyToRenderBuffer(){
    clearStaticFamilyVectors();
    filterEntitiesIntoStaticFamilies();
}

void EntityAdmin::updateMainThreadSystems(uint64_t dt){
    m_InputSystem.tick(dt);
    m_ConsoleSystem.tick(dt);
}

void EntityAdmin::update(uint64_t dt){
    
//    m_DebugPrintSystem.tick(dt);
//    m_ConsoleSystem.tick(dt);
    m_EditorSystem.tick(dt);
    
}

void EntityAdmin::render(){
    m_GreyBoxRenderSystem.render();
    m_EditorSystem.render();
    m_ConsoleSystem.render();
}

void EntityAdmin::teardown(){
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it){
        destroyEntity(it->first);
    }
    
    m_InputSystem.teardown();
}

void EntityAdmin::mainLoop(){
    
}

void EntityAdmin::clearFamilies(){
    getFamilyMap<CameraFamily>().clear();
    #include "clearFamiliesInclude.cpp"
}

void EntityAdmin::filterEntitiesIntoMutableFamilies(){
    for (std::pair<entityID, Entity*> pair : m_entities){
        Entity* e = pair.second;
        componentMask mask = e->m_mask;
        entityID eID = e->m_entityID;
        
        {
        if(ECSUtils::doesPassFilter(mask, Family<CameraFamily>::mask)){
            CameraFamily family = CameraFamily(eID, getComponent<TransformComponent>(eID), getComponent<CameraComponent>(eID));
            getFamilyMap<CameraFamily>().emplace(std::make_pair(eID, family));
            }
        }
        
        #include "filterEntitiesIntoMutableFamiliesInclude.cpp"
        
//        if(ECSUtils::doesPassFilter(mask, Family<DebugPrintableFamily>::mask)){
//            DebugPrintableFamily family = DebugPrintableFamily(eID, getComponent<DebugPrintComponent>(eID));
//            getFamilyMap<DebugPrintableFamily>().emplace(std::make_pair(eID, family));
//        }
    }
}

void EntityAdmin::filterEntitiesIntoStaticFamilies(){
    for (std::pair<entityID, Entity*> pair : m_entities){
        Entity* e = pair.second;
        componentMask mask = e->m_mask;
        entityID eID = e->m_entityID;
        
        {
        if(ECSUtils::doesPassFilter(mask, Family<CameraFamilyStatic>::mask)){
            CameraFamilyStatic family = CameraFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<CameraComponent>(eID));
            getFamilyStaticVector<CameraFamilyStatic>().push_back(family);
            }
        }
        #include "filterEntitiesIntoStaticFamiliesInclude.cpp"
    }
}

//TODO: Note that this doesn't work with the app sandbox on... I have to figure out how macos wants me to do file in the way it expects...

using json = nlohmann::json;
bool EntityAdmin::serializeByEntity(boost::filesystem::path outAbsolute){
    boost::filesystem::ofstream outfile;
    outfile.open(outAbsolute, std::ios_base::out);
    if(not outfile.is_open()){return false;}
    
    json out;
    
    out["entities"] = json::object();
    
    for(std::pair<entityID, Entity*> p : m_entities){
        entityID eID = p.first;
        std::string eIDStr = std::to_string(p.first);
        out["entities"][eIDStr] = json::object();
        
        for(auto it = componentsBegin(eID); it != componentsEnd(eID); ++it){
            out["entities"][eIDStr][ComponentIDToStringStruct::map.at(it.cID)] = (*it)->serialize();
        }

    }
    
    outfile << out.dump(4);
    
    
    outfile.close();
    return true;
}

bool EntityAdmin::deserializeByEntity(boost::filesystem::path inAbsolute){
    
    
    return true;
}

float seconds(uint64_t ms){ return ((float) ms) / 1000.0f;}
