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
#include "Components/ComponentsEnum.hpp"
#include "typedefs.h"

class Entity {
public:
    entityID m_entityID;
    
    unsigned long m_components[Components::ComponentsCount];
    
    Entity(entityID h) : m_entityID(h) {}
};

#endif /* Entity_hpp */
