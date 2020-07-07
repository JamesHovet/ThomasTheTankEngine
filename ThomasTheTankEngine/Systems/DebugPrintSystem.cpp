//
//  DebugPrint.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "DebugPrintSystem.hpp"

void DebugPrintSystem::tick(float dt){
    std::cout << "registered entities" << std::endl;
    for(entityID e : m_registered){
        std::cout << e << std::endl;
        auto idx = m_admin->m_entities.at(e)->m_components[Components::DebugInfoComponent];
        std::vector<DebugInfoComponent>* vec = (std::vector<DebugInfoComponent>*) m_admin->m_components_pool[Components::DebugInfoComponent];
        DebugInfoComponent c = vec->at(idx);
        std::cout << c.buf << std::endl;
    }
}

void DebugPrintSystem::init(){
    
}

DebugPrintSystem::~DebugPrintSystem(){
    
}
