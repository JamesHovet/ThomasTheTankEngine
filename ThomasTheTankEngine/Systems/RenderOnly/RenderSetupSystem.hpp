//
//  RenderSetupSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/10/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef RenderSetupSystem_hpp
#define RenderSetupSystem_hpp
#include "System.hpp"

class RenderSetupSystem : public System {
public:
    void init();
    void tick(uint64_t dt){};
    void render();
    
    RenderSetupSystem(EntityAdmin& admin) : System(admin) {};
    ~RenderSetupSystem(){};
private:
    void setupCamera();
};

#endif /* RenderSetupSystem_hpp */
