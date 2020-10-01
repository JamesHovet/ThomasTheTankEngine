//
//  EntityAdmin.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <ostream>
#include <istream>
#include <iostream>
#include <unordered_set>

#include "json.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include "FileUtils.hpp"

#include "Component.hpp"
#include "AllComponents.h"
#include "allFamilies.h"
#include "pool/object_pool.hpp"
#include "ECSUtils.hpp"
#include "Trace.hpp"

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
    m_BasicModelRenderSystem(*this),
    m_EditorSystem(*this),
    m_InputSystem(*this),
    m_ConsoleSystem(*this),
    m_RenderSetupSystem(*this),
    m_InputSingleton(*this)
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

Entity* EntityAdmin::tryCreateEntity(entityID eID){
    assert(m_entities.size() < MAX_ENTITIES);
    m_entities_dirty = true;
    if(m_entities.count(eID) != 0){
        return nullptr;
    }
    
    m_entities[eID].m_entityID = eID;
    m_entities[eID].m_mask.reset();
    m_component_maps.insert(std::make_pair(eID, std::unordered_map<componentID, ECSComponent*>()));
    
    return &m_entities[eID];
}

entityID EntityAdmin::createEntity(){
    assert(m_entities.size() < MAX_ENTITIES);
    m_entities_dirty = true;
    
    // pseudorandom entity ID
    entityID eID = rand() % MAX_ENTITIES;
    while(eID == 0 || m_entities.count(eID) != 0){ // 0 is reserved to represent "no entity"
        eID = rand() % MAX_ENTITIES;
    }
    
    m_entities[eID].m_entityID = eID;
    m_entities[eID].m_mask.reset();
    m_component_maps.insert(std::make_pair(eID, std::unordered_map<componentID, ECSComponent*>()));
    
    return eID;
}

bool EntityAdmin::entityExists(entityID eID){
    return m_entities.count(eID) != 0;
}

bool EntityAdmin::hasParent(entityID eID){
    return m_entities[eID].m_parentID != 0;
}

bool EntityAdmin::hasChildren(entityID eID){
    for(int i = 0; i < MAX_CHILDREN; i++){
        if(m_entities[eID].m_children[i] != NO_ENTITY){
            return true;
        }
    }
    return false;
}

bool EntityAdmin::isChildOf(entityID eID, entityID parent){
    return m_entities[eID].m_parentID == parent;
}

bool EntityAdmin::isParentOf(entityID eID, entityID child){
    for(int i = 0; i < MAX_CHILDREN; i++){
        if(m_entities[eID].m_children[i] == child){
            return true;
        }
    }
    return false;
}

bool EntityAdmin::addChild(entityID parent, entityID child){
    if(isParentOf(parent, child)){
        return true;
    }
    clearParent(child);

    for(int i = 0; i < MAX_CHILDREN; i++){
        if(m_entities[parent].m_children[i] == NO_ENTITY){
            m_entities[parent].m_children[i] = child;
            m_entities[child].m_parentID = parent;
            return true;
        }
    }
    printf("Failed to add %d to %d: too many children\n", child, parent);
    return false;
}

bool EntityAdmin::removeChild(entityID parent, entityID child){
    for(int i = 0; i < MAX_CHILDREN; i++){
        if(m_entities[parent].m_children[i] == child){
            m_entities[parent].m_children[i] = NO_ENTITY;
            m_entities[child].m_parentID = NO_ENTITY;
            return true;
        }
    }
    return false;
}

bool EntityAdmin::setParent(entityID child, entityID parent){
    if(isParentOf(parent, child)){
        return true;
    }
    clearParent(child);
    return addChild(parent, child);
}

bool EntityAdmin::clearParent(entityID child){
    entityID currentParent = m_entities[child].m_parentID;
    if (currentParent != NO_ENTITY){
        for(int i = 0; i < MAX_CHILDREN; i++){
            if(m_entities[currentParent].m_children[i] == child){
                m_entities[currentParent].m_children[i] = NO_ENTITY;
                m_entities[child].m_parentID = NO_ENTITY;
                return true;
            }
        }
    }
    return false;
}

void EntityAdmin::destroyEntity(entityID eID){
    m_entities_dirty = true;
    for(auto pair : m_component_maps.at(eID)){
        auto componentID = pair.first;
        auto componentPtr = pair.second;
        std::invoke(m_components_destuction_callbacks_array[componentID], componentPtr);
    }
    m_component_maps.erase(eID);
    m_entities.erase(m_entities.find(eID));
}

void EntityAdmin::destroyAllEntities(){
    m_entities_dirty = true;
    
    auto it = m_entities.begin();
    while(it != m_entities.end()){
        entityID eID = it->first;
        for(auto componentPair : m_component_maps.at(eID)){
            componentID cID = componentPair.first;
            ECSComponent* cPtr = componentPair.second;
            std::invoke(m_components_destuction_callbacks_array[cID], cPtr);
        }
        m_component_maps.erase(eID);
        it = m_entities.erase(it);
    }
}

void EntityAdmin::initAllSystems(){
    m_InputSystem.init();
    m_RenderSetupSystem.init();
    m_EditorSystem.init();
    m_ConsoleSystem.init();
    m_DebugPrintSystem.init();
    m_GreyBoxRenderSystem.init();
    m_BasicModelRenderSystem.init();

}

void EntityAdmin::setup(){
    initAllSystems();
    
    m_EditorSingleton.shouldUseEditorCamera = true;
    m_EditorSingleton.shouldTakeEditorInput = true;
    m_InputSingleton.shouldSendKeysTo = KEY_INPUT_MODE::EDITOR;
    
    return;
}

void EntityAdmin::loadTestScene(){
//    { // create camera
//        //@Remove: temporary test camera
//        entityID eID = this->createEntity();
//        TransformComponent& transformC = addComponent<TransformComponent>(eID);
//        transformC.m_position = glm::vec3(1.0f, 0.0f, -3.0f);
//        CameraComponent& cameraC = addComponent<CameraComponent>(eID);
////        cameraC.m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Main Camera";
//
//    }
    { // create testing boxes
        //@Remove: temporary test entities
        int numToAdd = 0;
        for(int i = 0; i < numToAdd; i++){
            entityID eID = this->createEntity();
//            DebugNameComponent& nameC = this->addComponent<DebugNameComponent>(eID);
            TransformComponent& transformC = this->addComponent<TransformComponent>(eID);
            GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(eID);
            AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(eID);

//            debugPrintC.m_enabled = false;

//            nameC.m_name = std::to_string(eID);
            transformC.m_position = glm::vec3(4.0f * ((float) i - (numToAdd / 2)) / (float) numToAdd);
            transformC.m_scale = glm::vec3(1.0f, 1.5f, 1.0f);
//                transformC.m_orientation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), glm::radians((float) random())));
//            transformC.m_orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
            boxC.m_color = glm::vec4(((float) i) * (1.0f / (float) numToAdd), 0.0, 0.0, 1.0f);
        }
    }
    {
        for(int i = 0; i < 3; i ++){
            entityID eID = this->createEntity();
            TransformComponent& trans = addComponent<TransformComponent>(eID);
            trans.m_position = glm::vec3((i - 1) * 10.0f, 0.0f, 0.0f);
            GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(eID);
            AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(eID);
            boxC.m_color = RGBA(1.0f, 0.0f, 0.0f, 1.0f);
            
            int thisRandomJ = rand() % 4;
            for (int j = 0; j < thisRandomJ; j ++){
                entityID child = createEntity();
                setParent(child, eID);
                TransformComponent& trans = addComponent<TransformComponent>(child);
                trans.m_scale = glm::vec3(0.5f);
                trans.m_position = glm::vec3((j - ((float)thisRandomJ / 2.0f)) * 2.0f + ((i - 1) * 10.0f), -2.5f, 0.0f);
                GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(child);
                AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(child);
                boxC.m_color = RGBA(0.0f, 1.0f, 0.0f, 1.0f);
                
                int thisRandomK = rand() % 4;
                for(int k = 0; k < thisRandomK; k++){
                    entityID grandchild = createEntity();
                    setParent(grandchild, child);
                    TransformComponent& trans = addComponent<TransformComponent>(grandchild);
                    trans.m_scale = glm::vec3(0.5f);
                    trans.m_position = glm::vec3((k - ((float)thisRandomK / 2.0f)) * 2.0f + (j - ((float)thisRandomJ / 2.0f)) * 3.0f + ((i - 1) * 10.0f), -5.0f, 0.0f);
                    GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(grandchild);
                    AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(grandchild);
                    boxC.m_color = RGBA(0.0f, 0.0f, 1.0f, 1.0f);
                }
            }
        }
    }
    
    
//    {
//        entityID eID = this->createEntity();
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Lion";
//        TransformComponent& trans = this->addComponent<TransformComponent>(eID);
//        trans.m_position = glm::vec3(0.0f, 0.0f, -2.0f);
//        BasicModelComponent& model = this->addComponent<BasicModelComponent>(eID);
//        model.m_model_name = "lion";
//        if(m_ModelCatalogSingleton.modelExists(model.m_model_name)){
//            AABBColliderComponent& bbox = this->addComponent<AABBColliderComponent>(eID);
//            bbox.m_AABB = this->m_ModelCatalogSingleton.getModel(model.m_model_name).bbox;
//        }
//
//    }
//    {
//        entityID eID = this->createEntity();
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Barrel";
//        TransformComponent& trans = this->addComponent<TransformComponent>(eID);
//        trans.m_position = glm::vec3(-5.0f, 0.0f, 0.0f);
//        BasicModelComponent& model = this->addComponent<BasicModelComponent>(eID);
//        model.m_model_name = "boxes";
//    }
//    {
//        entityID eID = this->createEntity();
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Suzanne";
//        TransformComponent& trans = this->addComponent<TransformComponent>(eID);
//        trans.m_position = glm::vec3(-5.0f, 0.0f, 0.0f);
//        BasicModelComponent& model = this->addComponent<BasicModelComponent>(eID);
//        model.m_model_name = "suzanne";
//        if(m_ModelCatalogSingleton.modelExists(model.m_model_name)){
//            AABBColliderComponent& bbox = this->addComponent<AABBColliderComponent>(eID);
//            bbox.m_AABB = this->m_ModelCatalogSingleton.getModel(model.m_model_name).bbox;
//        }
//    }
//    {
//        entityID eID = this->createEntity();
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Multi-Material";
//        TransformComponent& trans = this->addComponent<TransformComponent>(eID);
//        trans.m_position = glm::vec3(5.0f, 0.0f, 0.0f);
//        BasicModelComponent& model = this->addComponent<BasicModelComponent>(eID);
//        model.m_model_name = "multi-material";
//        AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(eID);
//        collisionC.m_AABB.max = glm::vec3(1.0f, 1.0f, 1.0f);
//        collisionC.m_AABB.min = glm::vec3(-1.0f, -1.0f, -1.0f);
//
//    }
//    {
//        entityID eID = this->createEntity();
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "column";
//        TransformComponent& trans = this->addComponent<TransformComponent>(eID);
//        trans.m_position = glm::vec3(5.0f, -2.0f, 0.0f);
//        trans.m_scale = glm::vec3(2.0f);
//        BasicModelComponent& model = this->addComponent<BasicModelComponent>(eID);
//        model.m_model_name = "column";
//        if(m_ModelCatalogSingleton.modelExists(model.m_model_name)){
//            AABBColliderComponent& bbox = this->addComponent<AABBColliderComponent>(eID);
//            bbox.m_AABB = this->m_ModelCatalogSingleton.getModel(model.m_model_name).bbox;
//        }
//    }
       
}

void EntityAdmin::defer(std::function<void ()> f){
    deferred_functions.push_back(f);
}

void EntityAdmin::exectueDeferred(){
    for(std::function<void (void)> function : deferred_functions){
        std::invoke(function);
    }
    deferred_functions.clear();
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
    TRACE_BEGIN("editor update", &m_EditorSystem);
    m_EditorSystem.tick(dt);
    TRACE_END("editor update", &m_EditorSystem);
    
}

void EntityAdmin::render(){
    m_RenderSetupSystem.render();
    TRACE_BEGIN("greybox render", &m_GreyBoxRenderSystem);
    m_GreyBoxRenderSystem.render();
    TRACE_END("greybox render", &m_GreyBoxRenderSystem);
    TRACE_BEGIN("Basic Model render", &m_BasicModelRenderSystem);
    m_BasicModelRenderSystem.render();
    TRACE_END("Basic Model render", &m_BasicModelRenderSystem);
    TRACE_BEGIN("editor render", &m_EditorSystem);
    m_EditorSystem.render();
    TRACE_END("editor render", &m_EditorSystem);
    m_ConsoleSystem.render();
}

void EntityAdmin::teardown(){
    destroyAllEntities();
    m_InputSystem.teardown();
}

void EntityAdmin::mainLoop(){
    
}

void EntityAdmin::clearFamilies(){
    getFamilyMap<CameraFamily>().clear();
    #include "clearFamiliesInclude.cpp"
}

void EntityAdmin::filterEntitiesIntoMutableFamilies(){
    for (std::pair<entityID, Entity> pair : m_entities){
        Entity e = pair.second;
        componentMask mask = e.m_mask;
        entityID eID = e.m_entityID;
        
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

static std::string prefix = "";

void EntityAdmin::addSubtreeIntoStaticFamilies(Entity e){
    entityID eID = e.m_entityID;
    componentMask mask = e.m_mask;
    
    {
    if(ECSUtils::doesPassFilter(mask, Family<CameraFamilyStatic>::mask)){
        CameraFamilyStatic family = CameraFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<CameraComponent>(eID));
        getFamilyStaticVector<CameraFamilyStatic>().push_back(family);
        }
    }
    #include "filterEntitiesIntoStaticFamiliesInclude.cpp"
    
    std::cout << prefix << e.m_entityID << std::endl;
    prefix.push_back('-');
    for(int i = 0; i < MAX_CHILDREN; i++){
        if(e.m_children[i] != 0){
            addSubtreeIntoStaticFamilies(m_entities.at(e.m_children[i]));
        }
    }
    prefix.pop_back();
}

void EntityAdmin::filterEntitiesIntoStaticFamilies(){
    std::cout << "Filtering:\n";
    for (std::pair<entityID, Entity> pair : m_entities){
        entityID eID = pair.first;
        Entity e = pair.second;
        if(e.m_parentID == 0){
            addSubtreeIntoStaticFamilies(e);
        }
    }
    std::cout << std::endl;
    
//
//    for (std::pair<entityID, Entity> pair : m_entities){
//        Entity e = pair.second;
//        componentMask mask = e.m_mask;
//        entityID eID = e.m_entityID;
//
//        {
//        if(ECSUtils::doesPassFilter(mask, Family<CameraFamilyStatic>::mask)){
//            CameraFamilyStatic family = CameraFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<CameraComponent>(eID));
//            getFamilyStaticVector<CameraFamilyStatic>().push_back(family);
//            }
//        }
//        #include "filterEntitiesIntoStaticFamiliesInclude.cpp"
//    }
}

//TODO: Note that this doesn't work with the app sandbox on... I have to figure out how macos wants me to do file in the way it expects...

using json = nlohmann::json;
bool EntityAdmin::serializeByEntityCompatability(boost::filesystem::path outAbsolute){
    boost::filesystem::ofstream outfile;
    outfile.open(outAbsolute, std::ios_base::out);
    if(! outfile.is_open()){return false;}
    
    json out = serializeByEntityInternal();
    
    outfile << out.dump(4);
    
    outfile.close();
    return true;
}

json::object_t EntityAdmin::serializeByEntityInternal(){
    json out;
    
    out["entities"] = json::object();
    
    for(std::pair<entityID, Entity> p : m_entities){
        entityID eID = p.first;
        std::string eIDStr = std::to_string(p.first);
        out["entities"][eIDStr] = json::object();
        
        for(auto it = componentsBegin(eID); it != componentsEnd(eID); ++it){
            out["entities"][eIDStr][ComponentIDToStringStruct::map.at(it.cID)] = (*it)->serialize();
        }

    }
    
    out["version"] = SERIALIZATION_VERSION;
    return out;
}

prototype EntityAdmin::createPrototypeFromEntity(entityID eID){
    prototype out;
    for(auto it = componentsBegin(eID); it != componentsEnd(eID); ++it){
        out[ComponentIDToStringStruct::map.at(it.cID)] = (*it)->serialize();
    }
    return out;
}

//TODO: make a not compatability version that is faster and doesn't have to do so many string compares
void EntityAdmin::populateEntityFromJson(entityID eID, json j){
    for(json::iterator componentIt = j.begin(); componentIt != j.end(); ++componentIt){
            if(componentIt.key() == "TransformComponent"){
                addComponent<TransformComponent>(eID, TransformComponent::deserialize(componentIt.value()));
            }
            else if (componentIt.key() == "DebugNameComponent"){
                addComponent<DebugNameComponent>(eID, DebugNameComponent::deserialize(componentIt.value()));
            }
            else if (componentIt.key() == "CameraComponent"){
                addComponent<CameraComponent>(eID, CameraComponent::deserialize(componentIt.value()));
            }
#include "deserializationCompatability.cpp"
        }
}

entityID EntityAdmin::createEntityFromPrototype(prototype proto){
    entityID eID = createEntity();
    populateEntityFromJson(eID, proto);
    return eID;
}

entityID EntityAdmin::duplicateEntity(entityID eID){
    entityID dupe = createEntity();
    populateEntityFromJson(dupe, createPrototypeFromEntity(eID));
    return dupe;
}

bool EntityAdmin::deserializeByEntityInternal(nlohmann::json::object_t in){
    for(json::iterator entityIt = in["entities"].begin(); entityIt != in["entities"].end(); ++entityIt){
        entityID eID = std::stoi(entityIt.key());
        bool couldCreateEntity = tryCreateEntity(eID);
        if(couldCreateEntity == false){
            return false;
        }
        populateEntityFromJson(eID, entityIt.value());
    }
    return true;
}

bool EntityAdmin::deserializeByEntityCompatability(boost::filesystem::path inAbsolute){
    
    boost::filesystem::ifstream infile;
    infile.open(inAbsolute, std::ios_base::in);
    if(! infile.is_open()){return false;}
    
    json in;
    
    infile >> in;
    
    bool out = deserializeByEntityInternal(in);
    
    infile.close();
    
    
    return out;
}

float seconds(uint64_t ms){ return ((float) ms) / 1000.0f;}
