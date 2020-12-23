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
    
    static unsigned int lines_VBO3d;
    static unsigned int lines_VAO3d;
    static unsigned int tris_VBO3d;
    static unsigned int tris_VAO3d;
    static unsigned int lines_VBO2d;
    static unsigned int lines_VAO2d;
    static unsigned int tris_VBO2d;
    static unsigned int tris_VAO2d;
    
    void draw3d();
    
    void draw2d();
    
};

#endif /* ImmediateRenderSystem_hpp */
