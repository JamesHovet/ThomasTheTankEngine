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
    entityID m_parentID = 0;
    entityID m_children[10]; // any bigger and we go over 32 bytes and then only one can fit on a cache line. 

    Entity(entityID eID){
        m_entityID = eID;
        m_mask.reset();
    }
};

#endif /* Entity_hpp */
