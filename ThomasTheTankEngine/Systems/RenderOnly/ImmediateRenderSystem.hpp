//
//  ImmediateRenderSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/16/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//
#pragma once
#ifndef ImmediateRenderSystem_hpp
#define ImmediateRenderSystem_hpp

#include "System.hpp"

class ImmediateRenderSystem : public System {
public:
    void init();
    void tick(uint64_t dt){};
    void render();
    
    ImmediateRenderSystem(EntityAdmin& admin) : System(admin) {};
    ~ImmediateRenderSystem(){};
private:
    
    static unsigned int lines_VBO;
    static unsigned int lines_VAO;
    static unsigned int tris_VBO;
    static unsigned int tris_VAO;
    
    void drawLines();
    void drawTris();
};

#endif /* ImmediateRenderSystem_hpp */
