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
#include "libs/imgui/imgui.h"

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
    DRight,
    LStickX,
    LStickY,
    RStickX,
    RStickY,
    LTAnalog,
    RTAnalog
};

const float STICK_DEAD_ZONE   = 0.2;
const float TRIGGER_DEAD_ZONE = 0.2;

void InputSystem::init(){
    InputSingleton& input = m_admin.m_InputSingleton;
    input.rawSDLState = SDL_GetKeyboardState(NULL);
    
    input.manager = new gainput::InputManager(false);
    
    input.padID = input.manager->CreateDevice<gainput::InputDevicePad>();
    input.pad = static_cast<gainput::InputDevicePad*>(input.manager->GetDevice(input.padID));
    input.keyboardID = input.manager->CreateDevice<gainput::InputDeviceKeyboard>();
    input.keyboard = static_cast<gainput::InputDeviceKeyboard*>(input.manager->GetDevice(input.keyboardID));
    input.mouseID = input.manager->CreateDevice<gainput::InputDeviceMouse>();
    input.mouse = static_cast<gainput::InputDeviceMouse*>(input.manager->GetDevice(input.mouseID));
 
    input.activeLine[0] = '\0';
    
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
    
    input.map->MapFloat(LStickX, input.padID, gainput::PadButtonLeftStickX);
    input.map->MapFloat(LStickY, input.padID, gainput::PadButtonLeftStickY);
    input.map->MapFloat(RStickX, input.padID, gainput::PadButtonRightStickX);
    input.map->MapFloat(RStickY, input.padID, gainput::PadButtonRightStickY);
    input.map->MapFloat(LTAnalog, input.padID, gainput::PadButtonAxis4);
    input.map->MapFloat(RTAnalog, input.padID, gainput::PadButtonAxis5);
    
    input.map->SetDeadZone(LStickX, STICK_DEAD_ZONE);
    input.map->SetDeadZone(LStickY, STICK_DEAD_ZONE);
    input.map->SetDeadZone(RStickX, STICK_DEAD_ZONE);
    input.map->SetDeadZone(RStickY, STICK_DEAD_ZONE);
    input.map->SetDeadZone(LTAnalog, TRIGGER_DEAD_ZONE);
    input.map->SetDeadZone(RTAnalog, TRIGGER_DEAD_ZONE);

}



void InputSystem::tick(uint64_t dt){
    InputSingleton& input     = m_admin.m_InputSingleton;
    EditorSingleton& edit     = m_admin.m_EditorSingleton;
    ConsoleSingleton& console = m_admin.m_ConsoleSingleton;

    // Process all SDL events
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(input.textInputMode){
            if(e.type == SDL_TEXTINPUT){
                printf("textin\n");
                if(input.activeLineCursor < MAX_INPUT_TEXT_LENGTH){
                    strcat(input.activeLine, e.text.text);
                    input.activeLineCursor += 1;
                }
            } else if (e.type == SDL_TEXTEDITING){
                printf("textedit\n");
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE){
                printf("backspace\n");
                if (input.activeLineCursor != 0){
                    input.activeLineCursor -= 1;
                    input.activeLine[input.activeLineCursor] = '\0';
                }
            }
        }
    }
    
    // Process all gainput inputs
    input.manager->Update(dt);
   
    //TODO: extend this system to get gamepad input sending between game and the debug camera
    switch (input.shouldSendKeysTo){
        case KEY_INPUT_MODE::GAME:
            if(input.keyboard->GetBool(gainput::KeyC) && not input.keyboard->GetBoolPrevious(gainput::KeyC)){
                input.priorShouldSendKeysTo = KEY_INPUT_MODE::GAME;
                input.shouldSendKeysTo = KEY_INPUT_MODE::EDITOR;
                
                edit.shouldUseEditorCamera = true;
                
            }
            if(input.keyboard->GetBool(gainput::KeyGrave) && not input.keyboard->GetBoolPrevious(gainput::KeyGrave)){
                input.priorShouldSendKeysTo = KEY_INPUT_MODE::GAME;
                input.shouldSendKeysTo = KEY_INPUT_MODE::CONSOLE;
                
                beginTextInput();
                console.consoleActive = true;
                printf("console should pop up\n");
            }
            break;
        case KEY_INPUT_MODE::EDITOR:
            if(input.keyboard->GetBool(gainput::KeyC) && not input.keyboard->GetBoolPrevious(gainput::KeyC)){
                input.shouldSendKeysTo = KEY_INPUT_MODE::GAME;
                
                edit.shouldUseEditorCamera = false;
                printf("leave editor\n");
            }
            if(input.keyboard->GetBool(gainput::KeyGrave) && not input.keyboard->GetBoolPrevious(gainput::KeyGrave)){
                input.priorShouldSendKeysTo = KEY_INPUT_MODE::EDITOR;
                input.shouldSendKeysTo = KEY_INPUT_MODE::CONSOLE;
               
                beginTextInput();
                console.consoleActive = true;
                printf("console should pop up\n");
            }
            break;
        case KEY_INPUT_MODE::CONSOLE:
            if(input.keyboard->GetBool(gainput::KeyGrave) && not input.keyboard->GetBoolPrevious(gainput::KeyGrave)){
                input.shouldSendKeysTo = input.priorShouldSendKeysTo;
                
                console.consoleActive = false;
                endTextInput();
                printf("leave console and return to where we were\n");
            }
            break;
    }
    
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
    
    input.LStickX   = input.map->GetFloat(LStickX);
    input.LStickY   = input.map->GetFloat(LStickY);
    input.RStickX   = input.map->GetFloat(RStickX);
    input.RStickY   = input.map->GetFloat(RStickY);
    input.LTAnalog  = input.map->GetFloat(LTAnalog);
    input.RTAnalog  = input.map->GetFloat(RTAnalog);
    
}

void InputSystem::teardown(){
    delete m_admin.m_InputSingleton.map;
    delete m_admin.m_InputSingleton.manager;
}


void InputSystem::beginTextInput(){
    InputSingleton& input = m_admin.m_InputSingleton;
    input.textInputMode = true;
    SDL_StartTextInput();
}

void InputSystem::endTextInput(){
    InputSingleton& input = m_admin.m_InputSingleton;
    input.textInputMode = false;
    SDL_StopTextInput();
}
