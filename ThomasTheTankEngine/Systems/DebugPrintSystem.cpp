//
//  DebugPrint.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "DebugPrintSystem.hpp"
#include <stdio.h>

void DebugPrintSystem::tick(float dt){
    std::cout << "registered entities:" << std::endl;
    for(entityID eID : m_registered){
        DebugNameComponent& nameC = m_admin.getComponent<DebugNameComponent>(eID);
        TransformComponent& transC = m_admin.getComponent<TransformComponent>(eID);
        glm::vec3 pos = transC.m_position;
        
        printf("[%10s] (%5.0f, %5.0f, %5.0f)\n", nameC.m_name.c_str(), pos.x, pos.y, pos.z);
    }
}

void DebugPrintSystem::init(){
    
}

DebugPrintSystem::~DebugPrintSystem(){
    
}
