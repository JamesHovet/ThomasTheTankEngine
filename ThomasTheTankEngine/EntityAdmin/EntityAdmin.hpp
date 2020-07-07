//
//  EntityAdmin.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef EntityAdmin_hpp
#define EntityAdmin_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <array>

#include "Component.hpp"
#include "Entity.hpp"
//#include "System.hpp"
#include "SystemsEnum.hpp"
#include "ComponentsEnum.hpp"
#include "AllComponents.hpp"

class System;

class EntityAdmin {
public:
    EntityAdmin();
    ~EntityAdmin();
    
    void setup();
    void update(float dt);
    void mainLoop(void);
    
    std::map<entityID, Entity*> m_entities;
    std::vector<System *> m_systems;
    std::array<void *, Components::ComponentsCount> m_components_pool;
};

#endif /* EntityAdmin_hpp */
