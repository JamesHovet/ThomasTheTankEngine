//
//  InputSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"
#include "InputSystem.hpp"

#include <SDL2/SDL.h>

enum Buttons {
    A,
    B,
    X,
    Y,
    start,
    back,
    RB,
    LB,
    RT,
    LT,
    LPush,
    RPush,
    DUp,
    DDown,
    DLeft,
    DRight
};

const int JOYSTICK_DEAD_ZONE = 8000;

void InputSystem::init(){
    InputSingleton& input = m_admin.m_InputSingleton;
    input.rawSDLState = SDL_GetKeyboardState(NULL);
    
    input.manager = new gainput::InputManager();
    
    input.padID = input.manager->CreateDevice<gainput::InputDevicePad>();
    input.pad = static_cast<gainput::InputDevicePad*>(input.manager->GetDevice(input.padID));
    input.keyboardID = input.manager->CreateDevice<gainput::InputDeviceKeyboard>();
    input.keyboard = static_cast<gainput::InputDeviceKeyboard*>(input.manager->GetDevice(input.keyboardID));
    input.mouseID = input.manager->CreateDevice<gainput::InputDeviceMouse>();
    input.mouse = static_cast<gainput::InputDeviceMouse*>(input.manager->GetDevice(input.mouseID));
    
    input.map = new gainput::InputMap(*input.manager);
    input.map->MapBool(A, input.padID, gainput::PadButtonA);
    input.map->MapBool(B, input.padID, gainput::PadButtonB);
    input.map->MapBool(X, input.padID, gainput::PadButtonX);
    input.map->MapBool(Y, input.padID, gainput::PadButtonY);
    input.map->MapBool(start, input.padID, gainput::PadButtonStart);
    input.map->MapBool(back, input.padID, gainput::PadButtonSelect);
    input.map->MapBool(RB, input.padID, gainput::PadButtonR1);
    input.map->MapBool(LB, input.padID, gainput::PadButtonL1);
    input.map->MapFloat(RT, input.padID, gainput::PadButtonAxis5);
    input.map->MapFloat(LT, input.padID, gainput::PadButtonAxis4);
    input.map->MapBool(LPush, input.padID, gainput::PadButtonL3);
    input.map->MapBool(RPush, input.padID, gainput::PadButtonR3);
    input.map->MapBool(DUp, input.padID, gainput::PadButtonUp);
    input.map->MapBool(DDown, input.padID, gainput::PadButtonDown);
    input.map->MapBool(DLeft, input.padID, gainput::PadButtonLeft);
    input.map->MapBool(DRight, input.padID, gainput::PadButtonRight);
    
}

void InputSystem::tick(float dt){
    InputSingleton& input = m_admin.m_InputSingleton;
    input.manager->Update(); // todo: add dt?
    
    float TRIGGER_DIGITAL_THRESHOLD = 0.7f;
    
    input.A      = input.map->GetBool(A);
    input.B      = input.map->GetBool(B);
    input.X      = input.map->GetBool(X);
    input.Y      = input.map->GetBool(Y);
    input.start  = input.map->GetBool(start);
    input.back   = input.map->GetBool(back);
    input.RB     = input.map->GetBool(RB);
    input.LB     = input.map->GetBool(LB);
    input.RT     = (input.map->GetFloat(RT) > TRIGGER_DIGITAL_THRESHOLD);
    input.LT     = (input.map->GetFloat(LT) > TRIGGER_DIGITAL_THRESHOLD);
    input.LPush  = input.map->GetBool(LPush);
    input.RPush  = input.map->GetBool(RPush);
    input.DUp    = input.map->GetBool(DUp);
    input.DDown  = input.map->GetBool(DDown);
    input.DLeft  = input.map->GetBool(DLeft);
    input.DRight = input.map->GetBool(DRight);
    
    
}

void InputSystem::teardown(){
    delete m_admin.m_InputSingleton.map;
    delete m_admin.m_InputSingleton.manager;
}
