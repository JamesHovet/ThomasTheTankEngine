//
//  EntityAdmin.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//
#pragma once
#ifndef EntityAdmin_hpp
#define EntityAdmin_hpp

#include <vector>
#include <unordered_map>
#include <array>
#include "pool/object_pool.hpp"
#include <boost/filesystem/path.hpp>

#include "typedefs.h"

#include "Component.hpp"
#include "ComponentsEnum.hpp"

#include "Entity.hpp"

#include "Family.hpp"
#include "FamiliesEnum.hpp"

#include "TimeSingleton.hpp"
#include "RenderSingleton.hpp"
#include "EditorSingleton.hpp"
#include "InputSingleton.hpp"
#include "ConsoleSingleton.hpp"
#include "ShaderCatalogSingleton.hpp"
#include "ModelCatalogSingleton.hpp"
#include "ImmediateRenderSingleton.hpp"

#include "System.hpp"
#include "AllSystems.hpp"




#define MAX_ENTITIES 2048
#define SERIALIZATION_VERSION "0.2"

struct entityIDHash {
    std::size_t operator()(const entityID eID) const {
        return (std::size_t)(eID % MAX_ENTITIES);
    }
};

struct entityIDComp {
    bool operator()(const entityID l, const entityID r) const {
        return l == r;
    }
};

class EntityAdmin {
    friend EditorSystem;
    friend ConsoleSystem;
public:
    EntityAdmin();
    ~EntityAdmin();
    
    void setup();
    void teardown();
    void filterIfNeeded();
    void exectueDeferred();
    void populateTime(uint64_t dt_ms){m_TimeSingleton.populateTime(dt_ms);};
    void updateMainThreadSystems();
    void update();
    void copyToRenderBuffer();
    void render();
    void mainLoop(void);
    int  getNumEntities(){return m_entities.size();}
    
    void loadTestScene();
    
    entityID createEntity();
    entityID createEntityFromPrototype(prototype proto);
    entityID createEntityFromPrototypeShallow(prototype proto);
    prototype createPrototypeFromEntity(entityID eID);
    prototype createPrototypeFromEntityShallow(entityID eID);
    entityID getParent(entityID eID);
    bool hasParent(entityID eID);
    bool hasChildren(entityID eID);
    bool isChildOf(entityID eID, entityID parent);
    bool isParentOf(entityID eID, entityID child);
    bool addChild(entityID parent, entityID child);
    bool removeChild(entityID parent, entityID child);
    bool setParent(entityID child, entityID parent);
    bool clearParent(entityID child);
    void defer(std::function<void (void)>);
    entityID duplicateEntity(entityID eID);
    entityID duplicateEntity(entityID eID, bool shouldParent);
    entityID duplicateEntityShallow(entityID eID);
    Entity* tryCreateEntity(entityID eID);
    void destroyEntity(entityID eID);
    bool entityExists(entityID eID);
    void destroyAllEntities();

    bool serializeByEntityCompatability(boost::filesystem::path outAbsolute);
    nlohmann::json::object_t serializeByEntityInternal();
    nlohmann::json::object_t serializeByEntityInternalHelper(entityID eID);
    bool deserializeByEntityCompatability(boost::filesystem::path inAbsolute);
    bool deserializeByEntityInternal(nlohmann::json::object_t obj);
    bool deserializeByEntityInternalHelper(nlohmann::json obj);
    bool deserializeEntityTree(entityID eID, nlohmann::json entityObj);

private:
    void initAllSystems();
    void filterEntitiesIntoMutableFamilies();
    void addSubtreeIntoStaticFamilies(Entity* e);
    void filterEntitiesIntoStaticFamilies();
    void clearFamilies();
    void clearStaticFamilyVectors();
    void populateComponentsFromJson(entityID eID, nlohmann::json j);
    bool populateEntityFromPrototype(entityID eID, prototype p);
public:
    template <typename T>
    T& addComponent(entityID eID){
        m_entities_dirty = true;
        
        int componentID = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        boost::object_pool<T>* this_pool = ((boost::object_pool<T>*)m_components_pool_array.at(componentID));
        T* out = this_pool->construct();
        ((ECSComponent*) out)->m_entity = m_entities.at(eID);
        
        m_component_maps.at(eID).insert(std::make_pair(componentID, out));
        m_entities.at(eID)->m_mask.set(componentID);
        
        return *out;
    }
    
    template <typename T>
    T& addComponent(entityID eID, T toCopy){
        // We want to copy everything from the component of type T *except* for the entity pointer that the vanilla addComponent<T>(eID) gives us. 
        T& out = addComponent<T>(eID);
        Entity* entityPtr = out.m_entity;
        out = toCopy;
        out.m_entity = entityPtr;
        return out;
    }
    
    template <typename T>
    void deferAdd(entityID eID){
        this->defer([this, eID](){
            this->addComponent<T>(eID);
        });
    }
    
    template <typename T>
    void deferAdd(entityID eID, T toCopy){
        this->defer([this, eID, toCopy](){
            T& out = this->addComponent<T>(eID);
            out = toCopy;
        });
    }
    
    template <typename T>
    void deferRemove(entityID eID){
        this->defer([this, eID](){
            removeComponent<T>(eID);
        });
    }
    
    void deferRemove(componentID cID, entityID eID){
        this->defer([this, cID, eID](){
            removeComponent(cID, eID);
        });
    }

    template <typename T>
    T* tryGetComponent(entityID eID){
        constexpr componentID cID = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        if(m_component_maps.count(eID) == 0){
            return nullptr;
        }
        auto& this_entity_map = m_component_maps.at(eID);
        if (this_entity_map.count(cID) != 0){
            return static_cast<T*>(this_entity_map.at(cID));
        }
        return nullptr;
    }
    
    template <typename T>
    T& getComponent(entityID eID){
        auto value = tryGetComponent<T>(eID);
        assert(value != nullptr);
        return *value;
    }
    
    template <typename T>
    T& getComponent(entityID eID) const {
        auto value = tryGetComponent<T>(eID);
        assert(value != nullptr);
        return *value;
    }
    
    class ComponentIter{
    public:
        EntityAdmin& m_admin;
        entityID eID;
        componentID cID;
        componentMask mask;
        
        ComponentIter(EntityAdmin& _m_admin, entityID _eID, componentID _cID) : m_admin(_m_admin){
            eID = _eID;
            cID = _cID;
            assert(m_admin.entityExists(eID));
            mask = m_admin.m_entities[eID]->m_mask;
            
            while(cID < NUM_COMPONENTS){
                if(mask[cID]){
                    break;
                }
                cID ++;
            }
            
        }

        ComponentIter& operator++(){
            while(cID < NUM_COMPONENTS){
                cID ++;
                if(mask[cID]){
                    break;
                }
            }
            return *this;
        }

        bool operator!=(const ComponentIter& other){
            return (eID != other.eID) || (cID != other.cID);
        }

        ECSComponent* operator*() const {
            assert(m_admin.m_component_maps.count(eID) != 0);
            auto this_entity_map = m_admin.m_component_maps.at(eID);
            if (this_entity_map.count(cID) != 0){
                return static_cast<ECSComponent*>(this_entity_map.at(cID));
            }
            return nullptr;
        }
        
    };
    
    ComponentIter componentsBegin(entityID eID){
        return ComponentIter(*this, eID, 0);
    }
    
    ComponentIter componentsEnd(entityID eID){
        return ComponentIter(*this, eID, NUM_COMPONENTS);
    }
    
    class ChildrenIter {
    public:
        EntityAdmin& m_admin;
        entityID eID;
        size_t index;
        
        ChildrenIter(EntityAdmin& _m_admin, entityID _eID, size_t _index) : m_admin(_m_admin){
            eID = _eID;
            index = _index;
            assert(m_admin.entityExists(eID));
            
            while(index < MAX_CHILDREN){
                if(m_admin.m_entities[eID]->m_children[index] != 0){
                    break;
                }
                index ++;
            }
        }
        
        ChildrenIter& operator++(){
            index ++;
            while(index < MAX_CHILDREN){
                if(m_admin.m_entities[eID]->m_children[index] != 0){
                    break;
                }
                index ++;
            }
            return *this;
        }
        
        bool operator!=(const ChildrenIter& other){
            return (eID != other.eID) || (index != other.index);
        }
        
        entityID operator*() const {
            entityID out = m_admin.m_entities[eID]->m_children[index];
            assert(out != NO_ENTITY);
            return out;
        }
    };
    
    ChildrenIter childrenBegin(entityID eID){
        return ChildrenIter(*this, eID, 0);
    }
    
    ChildrenIter childrenEnd(entityID eID){
        return ChildrenIter(*this, eID, MAX_CHILDREN);
    }
    
    
    void removeComponent(componentID cID, entityID eID){
        m_entities_dirty = true;
        auto& this_entity_map = m_component_maps.at(eID);
        ECSComponent* componentPtr = this_entity_map.at(cID);
        
        std::invoke(m_components_destuction_callbacks_array[cID], componentPtr);
        this_entity_map.erase(cID);
        
        
        m_entities.at(eID)->m_mask.reset(cID); // clear entity flag
    }
    
    template<typename T>
    void removeComponent(entityID eID){
        m_entities_dirty = true;
        
        componentID cID = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        auto& this_entity_map = m_component_maps.at(eID);
        ECSComponent* componentPtr = this_entity_map.at(cID);
        
        std::invoke(m_components_destuction_callbacks_array[cID], componentPtr);
        this_entity_map.erase(cID);
        
        
        m_entities.at(eID)->m_mask.reset(cID); // clear entity flag
    }
    

    template <typename T>
    std::unordered_map<entityID, T>& getFamilyMap() {
        constexpr familyID familyID = FamilyIndexTable::FamilyComponentIndex<T>::familyIndex;
        return *(static_cast<std::unordered_map<entityID, T>*>(m_families_maps_array[familyID]));
    }
    
    template <typename T>
    std::vector<T>& getFamilyStaticVector(){
        constexpr familyID familyID = FamilyIndexTable::FamilyComponentIndex<T>::familyIndex;
        return *(static_cast<std::vector<T>*>(m_families_maps_static_array[familyID]));
    }
//    getFamilyStaticVector
    
public:
    ShaderCatalogSingleton m_ShaderCatalogSingleton;
    ModelCatalogSingleton  m_ModelCatalogSingleton;
    TimeSingleton          m_TimeSingleton;
    
    RenderSingleton          m_RenderSingleton;
    EditorSingleton          m_EditorSingleton;
    InputSingleton           m_InputSingleton;
    ConsoleSingleton         m_ConsoleSingleton;
    ImmediateRenderSingleton m_ImmediateRenderSingleton;
private:
    InputSystem            m_InputSystem;
    EditorSystem           m_EditorSystem;
    ConsoleSystem          m_ConsoleSystem;
    DebugPrintSystem       m_DebugPrintSystem;
    GreyBoxRenderSystem    m_GreyBoxRenderSystem;
    BasicModelRenderSystem m_BasicModelRenderSystem;
    ImmediateRenderSystem  m_ImmediateRenderSystem;
    RenderSetupSystem      m_RenderSetupSystem;
    
private:
    std::unordered_map<entityID, std::unordered_map<componentID, ECSComponent *>> m_component_maps;
    
    std::unordered_map<entityID, Entity*, entityIDHash, entityIDComp> m_entities;
    std::array<Entity, MAX_ENTITIES> m_entity_storage;
    bool m_entities_dirty = true;
    
    std::array<void *, NUM_COMPONENTS> m_components_pool_array;
    std::array<std::function<void (void *)>, NUM_COMPONENTS> m_components_destuction_callbacks_array; // 
    std::vector<std::function<void (void)>> m_cleanup_callbacks;
    std::array<void *, NUM_FAMILIES> m_families_maps_array;
    std::array<void *, NUM_FAMILIES> m_families_maps_static_array;
    std::vector<std::function<void (void)>> deferred_functions;
    
    
//    std::array<void *, Singletons::SingletonsCount> m_singletons;
};

float seconds(uint64_t ms);
#endif /* EntityAdmin_hpp */


