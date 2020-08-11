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
#include "object_pool.hpp"
#include <boost/filesystem/path.hpp>

#include "typedefs.h"

#include "Component.hpp"
#include "ComponentsEnum.hpp"

#include "Entity.hpp"

#include "Family.hpp"
#include "FamiliesEnum.hpp"

#include "RenderSingleton.hpp"
#include "EditorSingleton.hpp"
#include "InputSingleton.hpp"
#include "ConsoleSingleton.hpp"
#include "ShaderCatalogSingleton.hpp"

#include "System.hpp"
#include "AllSystems.hpp"




#define MAX_ENTITIES 2048
#define SERIALIZATION_VERSION "0.1"

//class System;

// TODO: maybe create a forward declaration header for this to speed up compile time? I don't know exactly how that would work, but it might be worthwhile because of how interdependant everything is on this one header file. 

class EntityAdmin {
    friend EditorSystem;
    friend ConsoleSystem;
public:
    EntityAdmin();
    ~EntityAdmin();
    
    void setup();
    void teardown();
    void filterIfNeeded();
    void updateMainThreadSystems(uint64_t dt);
    void update(uint64_t dt);
    void copyToRenderBuffer();
    void render();
    void mainLoop(void);
    
    void loadTestScene();
    
    entityID createEntity();
    entityID createEntityFromPrototype(prototype proto);
    prototype createPrototypeFromEntity(entityID eID);
    entityID duplicateEntity(entityID eID);
    Entity* tryCreateEntity(entityID eID);
    void destroyEntity(entityID e);
    Entity* tryGetEntity(entityID e);
    Entity& getEntity(entityID e);
    void destroyAllEntities();

    bool serializeByEntityCompatability(boost::filesystem::path outAbsolute);
    nlohmann::json::object_t serializeByEntityInternal();
    bool deserializeByEntityCompatability(boost::filesystem::path inAbsolute);
    bool deserializeByEntityInternal(nlohmann::json::object_t obj);
    
private:
    void initAllSystems();
    void filterEntitiesIntoMutableFamilies();
    void filterEntitiesIntoStaticFamilies();
    void clearFamilies();
    void clearStaticFamilyVectors();
    void populateEntityFromJson(entityID eID, nlohmann::json j);
public:
    template <typename T>
    T& addComponent(entityID eID){
        m_entities_dirty = true;
        
        int componentID = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        boost::object_pool<T>* this_pool = ((boost::object_pool<T>*)m_components_pool_array.at(componentID));
        T* out = this_pool->construct();
        
        m_component_maps.at(eID).insert(std::make_pair(componentID, out));
        m_entities.at(eID)->m_mask.set(componentID);
        
        return *out;
    }
    
    template <typename T>
    T& addComponent(entityID eID, T toCopy){
        T& out = addComponent<T>(eID);
        out = toCopy;
        return out;
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
        ComponentIter(EntityAdmin& _m_admin, entityID _eID, componentID _cID) : m_admin(_m_admin){
            eID = _eID;
            cID = _cID;
            assert(m_admin.m_entities.count(eID) != 0);
            e = m_admin.m_entities.at(eID);
            
            while(cID < NUM_COMPONENTS){
                if(e->m_mask[cID]){
                    break;
                }
                cID ++;
            }
            
        }

        ComponentIter& operator++(){
            while(cID < NUM_COMPONENTS){
                cID ++;
                if(e->m_mask[cID]){
                    break;
                }
            }
            return *this;
        }

        bool operator!=(const ComponentIter& other){
            return (eID != other.eID) or (cID != other.cID);
        }

        ECSComponent* operator*() const {
            assert(m_admin.m_component_maps.count(eID) != 0);
            auto this_entity_map = m_admin.m_component_maps.at(eID);
            if (this_entity_map.count(cID) != 0){
                return static_cast<ECSComponent*>(this_entity_map.at(cID));
            }
            return nullptr;
        }
        
//    private:
        EntityAdmin& m_admin;
        Entity* e;
        entityID eID;
        componentID cID;
    };
    
    ComponentIter componentsBegin(entityID eID){
        return ComponentIter(*this, eID, 0);
    }
    
    ComponentIter componentsEnd(entityID eID){
        return ComponentIter(*this, eID, NUM_COMPONENTS);
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
    
    RenderSingleton        m_RenderSingleton;
    EditorSingleton        m_EditorSingleton;
    InputSingleton         m_InputSingleton;
    ConsoleSingleton       m_ConsoleSingleton;
private:
    InputSystem            m_InputSystem;
    EditorSystem           m_EditorSystem;
    ConsoleSystem          m_ConsoleSystem;
    DebugPrintSystem       m_DebugPrintSystem;
    GreyBoxRenderSystem    m_GreyBoxRenderSystem;
    RenderSetupSystem      m_RenderSetupSystem;
    
private:
    std::unordered_map<entityID, std::unordered_map<componentID, ECSComponent *>> m_component_maps;
    std::unordered_map<entityID, Entity*> m_entities;
    boost::object_pool<Entity> m_entity_pool;
    bool m_entities_dirty = true;
    
    std::array<void *, NUM_COMPONENTS> m_components_pool_array;
    std::array<std::function<void (void *)>, NUM_COMPONENTS> m_components_destuction_callbacks_array; // 
    std::vector<std::function<void (void)>> m_cleanup_callbacks;
    std::array<void *, NUM_FAMILIES> m_families_maps_array;
    std::array<void *, NUM_FAMILIES> m_families_maps_static_array;
    
    
//    std::array<void *, Singletons::SingletonsCount> m_singletons;
};

float seconds(uint64_t ms);
#endif /* EntityAdmin_hpp */


