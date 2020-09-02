//
//  BasicModelRenderSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef BasicModelRenderSystem_hpp
#define BasicModelRenderSystem_hpp

#include "System.hpp"

class BasicModelRenderSystem : public System {
public:
    void init();
    void renderDebugPanel();
    void tick(uint64_t dt){};
    void render();
    
    BasicModelRenderSystem(EntityAdmin& admin) : System(admin) {};
    ~BasicModelRenderSystem(){};
private:
};

#endif /* BasicModelRenderSystem_hpp */
