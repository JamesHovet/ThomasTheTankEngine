//
//  GreyBoxRenderSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef GreyBoxRenderSystem_hpp
#define GreyBoxRenderSystem_hpp

#include "System.hpp"

class GreyBoxRenderSystem : public System {
public:
    static unsigned int cube_VBO;
    static unsigned int cube_VAO;
    static unsigned int instanceVBO;
    
    void init();
    void tick(){};
    void render();
    
    GreyBoxRenderSystem(EntityAdmin& admin) : System(admin) {};
    ~GreyBoxRenderSystem(){};
private:
};

#endif /* GreyBoxRenderSystem_hpp */
