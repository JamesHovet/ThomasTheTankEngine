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
    std::cout << "registered entities" << std::endl;
    for(entityID eID : m_registered){
        std::cout << eID << std::endl;
        DebugInfoComponent& c = m_admin.getComponent<DebugInfoComponent>(eID);
//        DebugInfoComponent* c = m_admin.getComponent<DebugInfoComponent>(e, Components::DebugInfoComponent);
        std::cout << c.str << std::endl;
    }
}

void DebugPrintSystem::init(){
    
}

DebugPrintSystem::~DebugPrintSystem(){
    
}
