//
//  System.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef System_hpp
#define System_hpp


#include <set>
#include <stdio.h>
#include <iostream>

#include "typedefs.h"
#include "EntityAdmin.hpp"

class EntityAdmin;

class System {
public:
    virtual void tick(float dt) = 0;
    virtual void init(void) = 0;
    virtual void registerEntity(entityID e){
        m_registered.insert(e);
    }
    virtual void unregisterEntity(entityID e){
        m_registered.erase(e);
    }
//    virtual void SendMessage(Message *msg) = 0;
    virtual ~System(){};
    System(EntityAdmin * admin) : m_admin(admin) {};
protected:
    EntityAdmin* m_admin; 
    std::set<entityID> m_registered;
};

#endif /* System_hpp */
