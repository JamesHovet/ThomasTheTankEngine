//
//  Component.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Component_hpp
#define Component_hpp

#include "Entity.hpp"

struct Component {
    Entity* m_parentEntity;
};

#endif /* Component_hpp */
