//
//  System.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef System_hpp
#define System_hpp


#include <vector>
#include <stdio.h>
#include <iostream>

#include "typedefs.h"
//#include "EntityAdmin.hpp"

class EntityAdmin;

class System {
public:
    virtual void tick(float dt) = 0;
    virtual void init(void) = 0;
    virtual void render(void) = 0;
//    virtual void registerEntity(entityID e){
//        m_registered.push_back(e);
//    }
//    virtual void clear(){
//        m_registered.clear();
//    }
//    virtual void SendMessage(Message *msg) = 0;
    virtual ~System(){};
    System(EntityAdmin & admin) : m_admin(admin) {};
protected:
    EntityAdmin& m_admin;
//    std::vector<entityID> m_registered;
};

#endif /* System_hpp */
