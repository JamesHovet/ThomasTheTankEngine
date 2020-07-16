//
//  DebugPrint.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "DebugPrintSystem.hpp"
#include "DebugInfoComponent.hpp"

void DebugPrintSystem::tick(float dt){
    std::cout << "registered entities:" << std::endl;
    for(entityID eID : m_registered){
        DebugInfoComponent& c = m_admin.getComponent<DebugInfoComponent>(eID);
        std::cout << "[" << &c << "]: " <<  c.pos.x << " " << c.pos.y << " " << c.pos.z << std::endl;
    }
}

void DebugPrintSystem::init(){
    
}

DebugPrintSystem::~DebugPrintSystem(){
    
}
