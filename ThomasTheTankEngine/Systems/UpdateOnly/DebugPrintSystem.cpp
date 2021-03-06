//
//  DebugPrint.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"
#include "DebugPrintSystem.hpp"
#include "GreyBoxFamily.hpp"
#include <stdio.h>

void DebugPrintSystem::tick(){
    int count = 0;
    for(std::pair<entityID, GreyBoxFamily> p : m_admin.getFamilyMap<GreyBoxFamily>()){
        count++;
        GreyBoxFamily f = p.second;
        glm::vec4 pos = f.m_TransformComponent.getPosition();
        printf("[%4d]:(%4.0f, %4.0f, %4.0f) : %2.2f\n", f.eID, pos.x, pos.y, pos.z, f.m_GreyBoxComponent.m_color.r);
    }
    
    printf("\n%d\n", count);
      
}

void DebugPrintSystem::init(){
    
}
//
//DebugPrintSystem::~DebugPrintSystem(){
//    
//}
