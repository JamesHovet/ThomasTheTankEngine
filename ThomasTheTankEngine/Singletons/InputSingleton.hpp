//
//  InputSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef InputSingleton_hpp
#define InputSingleton_hpp

#include <SDL2/SDL.h>
#include <gainput/gainput.h>

//enum Buttons {
//    A,
//    B,
//    X,
//    Y,
//    start,
//    back,
//    RB,
//    LB,
//    RT,
//    LT,
//    LClick,
//    RClick,
//    DUp,
//    DDown,
//    DLeft,
//    DRight
//};

struct InputSingleton {

    gainput::InputManager* manager;
    gainput::InputMap* map;
    int padID = -1;
    int keyboardID = -1;
    int mouseID = -1;
    
    const Uint8 * rawState;
    
    
    bool A = false;
    bool B = false;
    bool X = false;
    bool Y = false;
    bool start = false;
    bool back = false;
    bool DUp  = false;
    bool DDown = false;
    bool DLeft = false;
    bool DRight = false;
};

#endif /* InputSingleton_hpp */
