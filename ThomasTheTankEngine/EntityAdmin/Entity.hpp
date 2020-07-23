//
//  Entity.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <algorithm>
#include "typedefs.h"
#include "Component.hpp"
#include <map>

class EntityAdmin;

struct Entity {
    friend EntityAdmin;
private:
//    std::map<int, Component*> m_componentMap;
public:
    componentMask m_mask;
    entityID m_entityID;

    Entity(entityID eID){
        m_entityID = eID;
        m_mask.reset();
    }

};

#endif /* Entity_hpp */
