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
#include <stack>

#include "json.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include "FileUtils.hpp"

#include "Component.hpp"
#include "allComponents.h"
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
    m_InputSingleton(*this),
    m_ImmediateRenderSystem(*this)
{
//    m_entities = std::unordered_map<entityID, Entity*>(MAX_ENTITIES);
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
    
    m_entity_storage[eID] = Entity(this, eID);
    m_entities[eID] = &m_entity_storage[eID];
    m_component_maps.insert(std::make_pair(eID, std::unordered_map<componentID, ECSComponent*>()));
    
    return m_entities[eID];
}

entityID EntityAdmin::createEntity(){
    assert(m_entities.size() < MAX_ENTITIES);
    m_entities_dirty = true;
    
    // pseudorandom entity ID
    entityID eID = rand() % MAX_ENTITIES;
    while(eID == 0 || m_entities.count(eID) != 0){ // 0 is reserved to represent "no entity"
        eID = rand() % MAX_ENTITIES;
    }
    
    m_entity_storage[eID] = Entity(this, eID);
    m_entities[eID] = &m_entity_storage[eID];
    m_component_maps.insert(std::make_pair(eID, std::unordered_map<componentID, ECSComponent*>()));
    
    return eID;
}

bool EntityAdmin::entityExists(entityID eID){
    return m_entities.count(eID) != 0;
}

bool EntityAdmin::hasParent(entityID eID){
    return m_entities[eID]->m_parentID != 0;
}

entityID EntityAdmin::getParent(entityID eID){
    return m_entities[eID]->m_parentID;
}

bool EntityAdmin::hasChildren(entityID eID){
    for(int i = 0; i < MAX_CHILDREN; i++){
        if(m_entities[eID]->m_children[i] != NO_ENTITY){
            return true;
        }
    }
    return false;
}

bool EntityAdmin::isChildOf(entityID eID, entityID parent){
    return m_entities[eID]->m_parentID == parent;
}

bool EntityAdmin::isParentOf(entityID eID, entityID child){
    for(int i = 0; i < MAX_CHILDREN; i++){
        if(m_entities[eID]->m_children[i] == child){
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
        if(m_entities[parent]->m_children[i] == NO_ENTITY){
            m_entities[parent]->m_children[i] = child;
            m_entities[child]->m_parentID = parent;
            return true;
        }
    }
    printf("Failed to add %d to %d: too many children\n", child, parent);
    return false;
}

bool EntityAdmin::removeChild(entityID parent, entityID child){
    for(int i = 0; i < MAX_CHILDREN; i++){
        if(m_entities[parent]->m_children[i] == child){
            m_entities[parent]->m_children[i] = NO_ENTITY;
            m_entities[child]->m_parentID = NO_ENTITY;
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
    entityID currentParent = m_entities[child]->m_parentID;
    if (currentParent != NO_ENTITY){
        for(int i = 0; i < MAX_CHILDREN; i++){
            if(m_entities[currentParent]->m_children[i] == child){
                m_entities[currentParent]->m_children[i] = NO_ENTITY;
                m_entities[child]->m_parentID = NO_ENTITY;
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
    if(hasChildren(eID)){
        for(auto it = childrenBegin(eID); it != childrenEnd(eID); ++it){
            destroyEntity(*it);
        }
    }
    if(hasParent(eID)){
        clearParent(eID);
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
    assert(m_entities.empty());
    assert(m_component_maps.empty());
}

void EntityAdmin::initAllSystems(){
    m_InputSystem.init();
    m_RenderSetupSystem.init();
    m_EditorSystem.init();
    m_ConsoleSystem.init();
    m_DebugPrintSystem.init();
    m_GreyBoxRenderSystem.init();
    m_ImmediateRenderSystem.init();
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
//        transformC.setPosition(glm::vec3(1.0f, 0.0f, -3.0f));
//        CameraComponent& cameraC = addComponent<CameraComponent>(eID);
////        cameraC.m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Main Camera";
//
//    }
    /*
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
            transformC.setLocalPosition(glm::vec3(4.0f * ((float) i - (numToAdd / 2)) / (float) numToAdd));
            transformC.setScale(glm::vec3(1.0f, 1.5f, 1.0f));
//                transformC.m_orientation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), glm::radians((float) random())));
//            transformC.m_orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
            boxC.m_color = glm::vec4(((float) i) * (1.0f / (float) numToAdd), 0.0, 0.0, 1.0f);
        }
    }
    */
    
//    {
//        entityID parent = this->createEntity();
//        entityID child = this->createEntity();
//        entityID grandchild = this->createEntity();
//        addChild(parent, child);
//        addChild(child, grandchild);
//
//        {
//            DebugNameComponent& nameC = addComponent<DebugNameComponent>(parent);
//            nameC.m_name = "parent";
//            TransformComponent& trans = addComponent<TransformComponent>(parent);
//            trans.setPosition(glm::vec3(0.0f));
//            GreyBoxComponent& boxC = addComponent<GreyBoxComponent>(parent);
//            boxC.m_color = RGBA(1.0f, 0.0f, 0.0f, 1.0f);
//            AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(parent);
//        }
//        {
//            DebugNameComponent& nameC = addComponent<DebugNameComponent>(child);
//            nameC.m_name = "child";
//            TransformComponent& trans = addComponent<TransformComponent>(child);
//            trans.setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
//            GreyBoxComponent& boxC = addComponent<GreyBoxComponent>(child);
//            boxC.m_color = RGBA(0.0f, 1.0f, 0.0f, 1.0f);
//            AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(child);
//        }
//
//        {
//            DebugNameComponent& nameC = addComponent<DebugNameComponent>(grandchild);
//            nameC.m_name = "grandchild";
//            TransformComponent& trans = addComponent<TransformComponent>(grandchild);
//            trans.setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
//            GreyBoxComponent& boxC = addComponent<GreyBoxComponent>(grandchild);
//            boxC.m_color = RGBA(0.0f, 0.0f, 1.0f, 1.0f);
//            AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(grandchild);
//        }
//
//
//    }
    
   
    {
        Entity* parentE = this->tryCreateEntity(1);
        entityID parent = 1;
        TransformComponent& parentXform = addComponent<TransformComponent>(parent);
        parentXform.setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        GreyBoxComponent& parentBoxC = this->addComponent<GreyBoxComponent>(parent);
        AABBColliderComponent& parentCollisionC = this->addComponent<AABBColliderComponent>(parent);
        parentBoxC.m_color = RGBA(1.0f, 0.0f, 0.0f, 1.0f);
        
        Entity* childE = this->tryCreateEntity(2);
        entityID child = 2;
        TransformComponent& childXform = addComponent<TransformComponent>(child);
        childXform.setLocalPosition(glm::vec3(1.0f, 0.0f, 0.0f));
        GreyBoxComponent& childBoxC = this->addComponent<GreyBoxComponent>(child);
        AABBColliderComponent& childCollisionC = this->addComponent<AABBColliderComponent>(child);
        childBoxC.m_color = RGBA(0.0f, 1.0f, 0.0f, 1.0f);
        
        Entity* grandchildE = this->tryCreateEntity(3);
        entityID grandchild = 3;
        TransformComponent& grandchildXform = addComponent<TransformComponent>(grandchild);
        grandchildXform.setLocalPosition(glm::vec3(1.0f, 0.0f, 0.0f));
        GreyBoxComponent& grandchildBoxC = this->addComponent<GreyBoxComponent>(grandchild);
        AABBColliderComponent& grandchildCollisionC = this->addComponent<AABBColliderComponent>(grandchild);
        grandchildBoxC.m_color = RGBA(0.0f, 0.0f, 1.0f, 1.0f);
        
        this->addChild(parent, child);
        this->addChild(child, grandchild);

    }
    
    /*
    {
        for(int i = 0; i < 1; i ++){
            entityID eID = this->createEntity();
            TransformComponent& trans = addComponent<TransformComponent>(eID);
            trans.setLocalPosition(glm::vec3((i - 0) * 10.0f, 0.0f, 0.0f));
            GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(eID);
            AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(eID);
            boxC.m_color = RGBA(1.0f, 0.0f, 0.0f, 1.0f);

            for (int j = 0; j < 10; j ++){
                entityID child = createEntity();
                setParent(child, eID);
                TransformComponent& trans = addComponent<TransformComponent>(child);
                trans.setScale(glm::vec3(0.5f));
                trans.setLocalPosition(glm::vec3((j - 5) * 2.0f, -2.5f, 0.0f));
                GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(child);
                AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(child);
                boxC.m_color = RGBA(0.0f, 1.0f, 0.0f, 1.0f);

                trans.getMat4();

                                            
                for(int k = 0; k < 10; k++){
                    entityID grandchild = createEntity();
                    setParent(grandchild, child);
                    TransformComponent& trans = addComponent<TransformComponent>(grandchild);
                    trans.setScale(glm::vec3(0.5f));
                    trans.setLocalPosition(glm::vec3((k - 5) * 2.0f, -5.0f, 0.0f));
                    GreyBoxComponent& boxC = this->addComponent<GreyBoxComponent>(grandchild);
                    AABBColliderComponent& collisionC = this->addComponent<AABBColliderComponent>(grandchild);
                    boxC.m_color = RGBA(0.0f, 0.0f, 1.0f, 1.0f);
                }
            }
        }
    }
    */
   
    
//    {
//        entityID eID = this->createEntity();
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Lion";
//        TransformComponent& trans = this->addComponent<TransformComponent>(eID);
//        trans.setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
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
//        trans.setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
//        BasicModelComponent& model = this->addComponent<BasicModelComponent>(eID);
//        model.m_model_name = "boxes";
//    }
//    {
//        entityID eID = this->createEntity();
//        DebugNameComponent& nameC = addComponent<DebugNameComponent>(eID);
//        nameC.m_name = "Suzanne";
//        TransformComponent& trans = this->addComponent<TransformComponent>(eID);
//        trans.setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
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
//        trans.setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
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
//        trans.setPosition(glm::vec3(5.0f, -2.0f, 0.0f));
//        trans.setScale(glm::vec3(2.0f));
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

void EntityAdmin::updateMainThreadSystems(){
    m_InputSystem.tick();
    m_ConsoleSystem.tick();
}

void EntityAdmin::update(){
//    m_DebugPrintSystem.tick();
//    m_ConsoleSystem.tick();
    TRACE_BEGIN("editor update", &m_EditorSystem);
    m_EditorSystem.tick();
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
    TRACE_BEGIN("immediate render", &m_ImmediateRenderSystem);
    m_ImmediateRenderSystem.render();
    TRACE_END("immediate render", &m_ImmediateRenderSystem);
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

static std::string prefix = "";
static glm::mat4 currentTransform = glm::mat4(1.0f);
static std::stack<glm::mat4> transformStack;

//void EntityAdmin::addSubtreeIntoStaticFamilies(Entity* e){
//    entityID eID = e->m_entityID;
//    componentMask mask = e->m_mask;
//
//
////    std::cout << prefix << e.m_entityID << std::endl;
////    prefix.push_back('-');
//    transformStack.push(currentTransform);
//
//    TransformComponent* transC = tryGetComponent<TransformComponent>(eID);
//
//    if(transC != nullptr && transC->m_dirty){
//        transC->makeClean();
////        glm::mat4 newMat4 = transC->getLocalMat4();
////        currentTransform = currentTransform * newMat4;
////        transC->m_cachedMat4 = currentTransform;
////        transC->m_dirty = false;
//    }
//
//    // do the actual filtering
//    {
//        {
//        if(ECSUtils::doesPassFilter(mask, Family<CameraFamilyStatic>::mask)){
//            CameraFamilyStatic family = CameraFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<CameraComponent>(eID));
//            getFamilyStaticVector<CameraFamilyStatic>().push_back(family);
//            }
//        }
//        #include "filterEntitiesIntoStaticFamiliesInclude.cpp"
//    }
//
//    // recurse
//    for(int i = 0; i < MAX_CHILDREN; i++){
//        if(e->m_children[i] != 0){
//            addSubtreeIntoStaticFamilies(m_entities.at(e->m_children[i]));
//        }
//    }
//    currentTransform = transformStack.top();
//    transformStack.pop();
////    prefix.pop_back();
//}

void EntityAdmin::filterEntitiesIntoStaticFamilies(){
//    std::cout << "Filtering:\n";
    for (std::pair<entityID, Entity*> pair : m_entities){
        entityID eID = pair.first;
        Entity* e = pair.second;
        componentMask mask = e->m_mask;
        TransformComponent* transC = tryGetComponent<TransformComponent>(eID);
        
        if(transC != nullptr && transC->m_dirty){
            transC->makeClean();
        }
//        if(e->m_parentID == 0){
//            addSubtreeIntoStaticFamilies(e);
        {
            {
            if(ECSUtils::doesPassFilter(mask, Family<CameraFamilyStatic>::mask)){
                CameraFamilyStatic family = CameraFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<CameraComponent>(eID));
                getFamilyStaticVector<CameraFamilyStatic>().push_back(family);
                }
            }
            #include "filterEntitiesIntoStaticFamiliesInclude.cpp"
        }
            
//        }
    }
//    std::cout << std::endl;
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

json::object_t EntityAdmin::serializeByEntityInternalHelper(entityID eID){
    json::object_t out = json::object();
    out["components"] = json::object();
    
    for(auto it = componentsBegin(eID); it != componentsEnd(eID); ++it){
        out["components"][ComponentIDToStringStruct::map.at(it.cID)] = (*it)->serialize();
    }
    out["children"] = json::object();
    Entity* e = m_entities.at(eID);
    for(int childIndex = 0; childIndex < MAX_CHILDREN; childIndex++){
        if(e->m_children[childIndex] != NO_ENTITY){
            entityID child = e->m_children[childIndex];
            out["children"][std::to_string(child)] = serializeByEntityInternalHelper(child);
        }
    }
    return out;
}

json::object_t EntityAdmin::serializeByEntityInternal(){
    json out;
    
    out["entities"] = json::object();
    
    for(std::pair<entityID, Entity*> p : m_entities){
        if(!p.second->hasParent()){ // only root elements
            out["entities"][std::to_string(p.first)] = serializeByEntityInternalHelper(p.first);
        }
    }
    
    out["version"] = SERIALIZATION_VERSION;
    return out;
}

prototype EntityAdmin::createPrototypeFromEntityShallow(entityID eID){
    prototype out;
    for(auto it = componentsBegin(eID); it != componentsEnd(eID); ++it){
        out[ComponentIDToStringStruct::map.at(it.cID)] = (*it)->serialize();
    }
    return out;
}

prototype EntityAdmin::createPrototypeFromEntity(entityID eID){
    return serializeByEntityInternalHelper(eID);
}

//TODO: make a not compatability version that is faster and doesn't have to do so many string compares
void EntityAdmin::populateComponentsFromJson(entityID eID, json j){
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
            else {
                std::cout << "DESERIALIZATION FAILED: NO COMPONENT WITH NAME " << componentIt.key() << std::endl;
            }
        }
}

bool EntityAdmin::populateEntityFromPrototype(entityID eID, prototype p){
    populateComponentsFromJson(eID, p["components"]);
//    std::cout << p["children"] << std::endl;
    for(json::iterator childrenIt = p["children"].begin(); childrenIt != p["children"].end(); ++childrenIt){
        entityID childID = createEntity();
        if(!populateEntityFromPrototype(childID, childrenIt.value())){
            return false;
        }
        addChild(eID, childID);
    }
    return true;
}

entityID EntityAdmin::createEntityFromPrototype(prototype proto){
    entityID eID = createEntity();
    if(populateEntityFromPrototype(eID, proto)){
        return eID;
    }
    assert(false);
    return NO_ENTITY;
}

entityID EntityAdmin::createEntityFromPrototypeShallow(prototype proto){
    entityID eID = createEntity();
    populateComponentsFromJson(eID, proto);
    return eID;
}

entityID EntityAdmin::duplicateEntity(entityID eID, bool shouldParent){
    prototype proto = createPrototypeFromEntity(eID);
//    std::cout << proto << std::endl;
    entityID out = createEntityFromPrototype(proto);
    if(shouldParent){
        entityID parent = getParent(eID);
        if(parent != NO_ENTITY){
            addChild(parent, out);
        }
    }
    return out;
}

entityID EntityAdmin::duplicateEntity(entityID eID){
    return duplicateEntity(eID, true);
}

entityID EntityAdmin::duplicateEntityShallow(entityID eID){
    entityID dupe = createEntity();
    populateComponentsFromJson(dupe, createPrototypeFromEntity(eID));
    return dupe;
}

bool EntityAdmin::deserializeEntityTree(entityID eID, nlohmann::json entityObj){
    populateComponentsFromJson(eID, entityObj["components"]);
    for(json::iterator childrenIt = entityObj["children"].begin(); childrenIt != entityObj["children"].end(); ++childrenIt){
        entityID childID = std::stoi(childrenIt.key());
        if(deserializeEntityTree(childID, childrenIt.value())){
            return false;
        }
        addChild(eID, childID);
    }
    return true;
    /*
     populateComponentsFromJson(eID, p["components"]);
     std::cout << p["children"] << std::endl;
     for(json::iterator childrenIt = p["children"].begin(); childrenIt != p["children"].end(); ++childrenIt){
         entityID childID = createEntity();
         if(!populateEntityFromPrototype(childID, childrenIt.value())){
             return false;
         }
         addChild(eID, childID);
     }
     return true;
     */
}

bool EntityAdmin::deserializeByEntityInternalHelper(nlohmann::json dict_of_entities){
    for(json::iterator entityIt = dict_of_entities.begin(); entityIt != dict_of_entities.end(); ++entityIt){
        entityID eID;
        eID = std::stoi(entityIt.key());
        bool couldCreateEntity = tryCreateEntity(eID);
        if(!couldCreateEntity){
            return false;
        }
        populateEntityFromPrototype(eID, entityIt.value());
    }
    return true;
}

bool EntityAdmin::deserializeByEntityInternal(nlohmann::json::object_t in){
    return deserializeByEntityInternalHelper(in["entities"]);
}

bool EntityAdmin::deserializeByEntityCompatability(boost::filesystem::path inAbsolute){
    
    boost::filesystem::ifstream infile;
    infile.open(inAbsolute, std::ios_base::in);
    if(! infile.is_open()){return false;}
    
    json in;
    
    infile >> in;
    
    bool out;
    out = deserializeByEntityInternal(in);
    
    infile.close();
    
    return out;
}

float seconds(uint64_t ms){ return ((float) ms) / 1000.0f;}
