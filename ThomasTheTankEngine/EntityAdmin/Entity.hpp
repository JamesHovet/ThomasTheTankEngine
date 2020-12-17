//
//  Entity.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//
#pragma once
#ifndef Entity_hpp
#define Entity_hpp

#include "typedefs.h"

class EntityAdmin;

struct Entity {
    friend EntityAdmin;
private:
//    std::map<int, Component*> m_componentMap;
public:
    componentMask m_mask;
    entityID m_entityID;
    entityID m_parentID = NO_ENTITY;
    entityID m_children[MAX_CHILDREN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // any bigger and we go over 32 bytes and then only one can fit on a cache line.
    EntityAdmin* m_admin;

    Entity() : m_entityID(NO_ENTITY){}
    
    Entity(EntityAdmin * _admin, entityID eID) : m_entityID(eID), m_admin(_admin){
        m_mask.reset();
    }
    
    bool hasParent(){
        return m_parentID != NO_ENTITY;
    }
};

#endif /* Entity_hpp */
