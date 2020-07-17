//
//  DebugPrint.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef DebugPrintSystem_hpp
#define DebugPrintSystem_hpp

#include "System.hpp"
#include "DebugNameComponent.hpp"
#include "TransformComponent.hpp"

class DebugPrintSystem : public System {
public:
    void tick(float dt);
    void init();
    DebugPrintSystem(EntityAdmin& admin) : System(admin) {};
    ~DebugPrintSystem();
};

#endif /* DebugPrintSystem_hpp */
