//
//  ConsoleSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/27/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef ConsoleSystem_hpp
#define ConsoleSystem_hpp
#include "System.hpp"
#include "ConsoleCommands.hpp"

class ConsoleSystem : public System {
public:
    ConsoleSystem(EntityAdmin& admin) : System(admin) {};
    ~ConsoleSystem(){};
    
    void init();
    void tick();
    void render();
};

#endif /* ConsoleSystem_hpp */
