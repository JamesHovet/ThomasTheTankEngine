//
//  EntityAdmin.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef EntityAdmin_hpp
#define EntityAdmin_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <unordered_set>
#include "object_pool.hpp"
#include <functional>

#include "Component.hpp"
#include "ComponentsEnum.hpp"

#include "Entity.hpp"

#include "Family.hpp"
#include "FamiliesEnum.hpp"
//#include "System.hpp"
#include "AllSystems.hpp"

#include "SingletonsEnum.hpp"


#define MAX_ENTITIES 2048

//class System;

class EntityAdmin {
public:
    EntityAdmin();
    ~EntityAdmin();
    
    void setup();
    void teardown();
    void update(float dt);
    void mainLoop(void);
    
    entityID createEntity();
    void destroyEntity(entityID e);
    
    void filterEntitiesIntoFamilies();
    
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
    T* tryGetComponent(entityID eID){
        constexpr componentID cID = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        assert(m_component_maps.count(eID) != 0);
        auto this_entity_map = m_component_maps.at(eID);
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
    
    template<typename T>
    void removeComponent(entityID eID){
        m_entities_dirty = true;
        
        componentID cID = ComponentIndexTable::RetrieveComponentIndex<T>::componentIndex;
        auto this_entity_map = m_component_maps.at(eID);
        Component* componentPtr = this_entity_map.at(cID);
        
        std::invoke(m_components_destuction_callbacks_array[cID], componentPtr);
        this_entity_map.erase(cID);
        
        m_entities.at(eID)->m_mask.reset(cID); // clear entity flag
    }
    

    template <typename T>
    std::unordered_map<entityID, T>& getFamilyMap() {
        constexpr familyID familyID = FamilyIndexTable::FamilyComponentIndex<T>::familyIndex;
        return *(static_cast<std::unordered_map<entityID, T>*>(m_families_maps_array[familyID]));
    }
    
    
private:
    std::unordered_map<entityID, std::unordered_map<componentID, Component *>> m_component_maps;
    std::unordered_map<entityID, Entity*> m_entities;
    boost::object_pool<Entity> m_entity_pool;
    bool m_entities_dirty = true;
    
    std::array<void *, NUM_COMPONENTS> m_components_pool_array;
    std::array<std::function<void (void *)>, NUM_COMPONENTS> m_components_destuction_callbacks_array; // 
    std::vector<std::function<void (void)>> m_cleanup_callbacks;
    std::array<void *, NUM_FAMILIES> m_families_maps_array;
    
    DebugPrintSystem m_DebugPrintSystem;
    
    
//    std::array<void *, Singletons::SingletonsCount> m_singletons;
};
#endif /* EntityAdmin_hpp */
