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
//    LPush,
//    RPush,
//    DUp,
//    DDown,
//    DLeft,
//    DRight,
//    LStickX,
//    LStickY,
//    RStickX,
//    RStickY,
//    LTAnalog,
//    RTAnalog
//};

const float STICK_DEAD_ZONE   = 0.2;
const float TRIGGER_DEAD_ZONE = 0.2;

void InputSystem::init(){
    InputSingleton& input = m_admin.m_InputSingleton;
    input.rawSDLState = SDL_GetKeyboardState(NULL);
}



void InputSystem::tick(uint64_t dt){
    InputSingleton& input     = m_admin.m_InputSingleton;
    EditorSingleton& edit     = m_admin.m_EditorSingleton;
    ConsoleSingleton& console = m_admin.m_ConsoleSingleton;

    // Process all SDL events
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        //textinput
        if(input.textInputMode){
            if(e.type == SDL_TEXTINPUT){
                // printf("textin\n");
                if(input.activeLineCursor < MAX_INPUT_TEXT_LENGTH){
                    if(e.text.text[0] != '`'){
                        strcat(input.activeLine, e.text.text);
                        input.activeLineCursor += 1;
                    }
                }
            } else if (e.type == SDL_TEXTEDITING){
                // printf("textedit\n");
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE){
                // printf("backspace\n");
                if (input.activeLineCursor != 0){
                    input.activeLineCursor -= 1;
                    input.activeLine[input.activeLineCursor] = '\0';
                }
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN){
                // printf("return\n");
                input.lineCommitted = true;
            }
        }
        
        //mouse input
        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT){
                input.hasPendingClick = true;
                input.clickWindowSpace = glm::vec2(e.button.x, e.button.y);
                float fx      = (float) e.button.x;
                float fy      = (float) e.button.y;
                float fwidth  = (float) m_admin.m_RenderSingleton.SCREEN_WIDTH;
                float fheight = (float) m_admin.m_RenderSingleton.SCREEN_HEIGHT;
                input.clickViewportSpace = glm::vec2((fx * 2.0f / fwidth) - 1.0f,
                                                     1.0f - (fy * 2.0f / fheight));
//                printf("window space: %f, %f\n", input.clickWindowSpace.x, input.clickWindowSpace.y);
//                printf("viewport space: %f, %f\n", input.clickViewportSpace.x, input.clickViewportSpace.y);
            }
        }
        
        // window resizing
        if(e.type == SDL_WINDOWEVENT){
            if (e.window.event == SDL_WINDOWEVENT_RESIZED){
                m_admin.m_RenderSingleton.SCREEN_WIDTH  = e.window.data1;
                m_admin.m_RenderSingleton.SCREEN_HEIGHT = e.window.data2;
                glViewport(0, 0, m_admin.m_RenderSingleton.SCREEN_WIDTH, m_admin.m_RenderSingleton.SCREEN_HEIGHT);
            }
        }
        
        if(e.type == SDL_KEYDOWN){
//            if(e.key.keysym.scancode == SDL_SCANCODE_GRAVE)
            switch (input.shouldSendKeysTo){
                case KEY_INPUT_MODE::GAME:
                    if(e.key.keysym.scancode == SDL_SCANCODE_C){
                        input.priorShouldSendKeysTo = KEY_INPUT_MODE::GAME;
                        input.shouldSendKeysTo = KEY_INPUT_MODE::EDITOR;
                        
                        edit.shouldUseEditorCamera = true;
                        
                    }
                    if(e.key.keysym.scancode == SDL_SCANCODE_GRAVE){
                        input.priorShouldSendKeysTo = KEY_INPUT_MODE::GAME;
                        input.shouldSendKeysTo = KEY_INPUT_MODE::CONSOLE;
                        
                        beginTextInput();
                        console.consoleActive = true;
                        // printf("console should pop up\n");
                    }
                    break;
                case KEY_INPUT_MODE::EDITOR:
                    if(e.key.keysym.scancode == SDL_SCANCODE_C){
                        input.shouldSendKeysTo = KEY_INPUT_MODE::GAME;
                        
                        edit.shouldUseEditorCamera = false;
                        // printf("leave editor\n");
                    }
                    if(e.key.keysym.scancode == SDL_SCANCODE_GRAVE){
                        input.priorShouldSendKeysTo = KEY_INPUT_MODE::EDITOR;
                        input.shouldSendKeysTo = KEY_INPUT_MODE::CONSOLE;
                       
                        beginTextInput();
                        console.consoleActive = true;
                        // printf("console should pop up\n");
                    }
                    if(e.key.keysym.scancode == SDL_SCANCODE_G){
                        edit.usingLocalWorldSpace = !edit.usingLocalWorldSpace;
                    }
                    break;
                case KEY_INPUT_MODE::CONSOLE:
                    if(e.key.keysym.scancode == SDL_SCANCODE_GRAVE){
                        input.shouldSendKeysTo = input.priorShouldSendKeysTo;
                        
                        console.consoleActive = false;
                        endTextInput();
                        // printf("leave console and return to where we were\n");
                    }
                    break;
            }
        }
    }
    
}

void InputSystem::teardown(){

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
