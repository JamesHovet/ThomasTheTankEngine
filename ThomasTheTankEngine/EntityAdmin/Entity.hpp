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
#include <algorithm>

struct Entity {
    entityID m_entityID;
    
    int m_components[Components::ComponentsCount];
    
    Entity(entityID eID){
        m_entityID = eID;
        std::fill(m_components, m_components + Components::ComponentsCount, -1);
    }
};

#endif /* Entity_hpp */
