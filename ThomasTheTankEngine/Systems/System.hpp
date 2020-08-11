//
//  System.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef System_hpp
#define System_hpp

#include "typedefs.h"

class EntityAdmin;

class System {
public:
    virtual void tick(uint64_t dt) = 0;
    virtual void init(void) = 0;
    virtual void render(void) = 0;
    virtual ~System(){};
    System(EntityAdmin & admin) : m_admin(admin) {};
protected:
    EntityAdmin& m_admin;
};

#endif /* System_hpp */
