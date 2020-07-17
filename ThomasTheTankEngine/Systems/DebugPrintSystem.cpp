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
//    std::cout << "registered entities:" << std::endl;
    float sum = 0;
    for(entityID eID : m_registered){
        DebugNameComponent& nameC = m_admin.getComponent<DebugNameComponent>(eID);
        TransformComponent& transC = m_admin.getComponent<TransformComponent>(eID);
        glm::vec3& pos = transC.m_position;
        
//        pos.y = dt;
//        sum += pos.y;
    }
    
}

void DebugPrintSystem::init(){
    
}

DebugPrintSystem::~DebugPrintSystem(){
    
}
