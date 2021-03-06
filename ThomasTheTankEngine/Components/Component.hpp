//
//  Component.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef Component_hpp
#define Component_hpp


#include "typedefs.h"
#include "imgui/imgui.h"
#include "ImGuiUtils.hpp"
#include "SerializationUtils.hpp"
#include "json.hpp"

#define MAX_COMPONENT_TYPES 64

//#include "Entity.hpp"
class Entity;

using json = nlohmann::json;

class EntityAdmin;
struct ECSComponent {
    virtual void imDisplay(EntityAdmin* m_admin){};
    virtual componentID getComponentIndex() = 0;
    virtual json::object_t serialize() = 0;
    Entity* m_entity;
};

// a bit of a hack to allow lookup of a component index using <> notation.
// usage: `ComponentIndexTable::RetrieveComponentIndex<ComponentName>`
namespace ComponentIndexTable {
    template <typename T>
    struct RetrieveComponentIndex {
        static constexpr componentID componentIndex = T::componentIndex;
    };
}

#endif /* Component_hpp */
