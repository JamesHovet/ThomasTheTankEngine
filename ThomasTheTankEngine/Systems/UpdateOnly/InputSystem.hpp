//
//  InputSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef InputSystem_hpp
#define InputSystem_hpp

#include "System.hpp"

class InputSystem : public System {
public:
    void init();
    void tick();
    void render(){};
    void teardown();
   
    void beginTextInput();
    void endTextInput();
    
    InputSystem(EntityAdmin& admin) : System(admin) {};
    ~InputSystem(){};
};


#endif /* InputSystem_hpp */
