//
//  DebugPrint.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"
#include "DebugPrintSystem.hpp"
#include <stdio.h>

void DebugPrintSystem::tick(float dt){
    
    for(GreyBoxFamily f : m_admin.getFamilyVector<GreyBoxFamily>()){
        auto pos = f.m_TransformComponent.m_position;
        printf("(%4.0f, %4.0f, %4.0f) : %2.2f\n", pos.x, pos.y, pos.z, f.m_GreyBoxComponent.m_color.r);
    }
    
//    std::cout << "registered entities:" << std::endl;
//    for(entityID eID : m_registered){
//        DebugNameComponent& nameC = m_admin.getComponent<DebugNameComponent>(eID);
//        TransformComponent& transC = m_admin.getComponent<TransformComponent>(eID);
//        glm::vec3& pos = transC.m_position;
        
//        pos.y = dt;
//        sum += pos.y;
//    }
    
}

void DebugPrintSystem::init(){
    
}
//
//DebugPrintSystem::~DebugPrintSystem(){
//    
//}
