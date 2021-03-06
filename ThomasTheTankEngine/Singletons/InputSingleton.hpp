//
//  InputSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef InputSingleton_hpp
#define InputSingleton_hpp

#include <SDL.h>
#include "typedefs.h"
#include <glm/gtx/norm.hpp>

#define DRAG_ACTIVATION_RADIUS 10.0f

enum class KEY_INPUT_MODE {
    GAME,
    EDITOR,
    CONSOLE
};

enum class PAD_INPUT_MODE {
    GAME,
    EDITOR
};

#define MAX_INPUT_TEXT_LENGTH 256

struct InputSingleton {
protected:
    EntityAdmin& m_admin;
public:
    InputSingleton(EntityAdmin& _m_admin) : m_admin(_m_admin){}
    
    int padID = -1;
    int keyboardID = -1;
    int mouseID = -1;

    const Uint8 * rawSDLState;
    
    KEY_INPUT_MODE shouldSendKeysTo = KEY_INPUT_MODE::GAME;
    KEY_INPUT_MODE priorShouldSendKeysTo = KEY_INPUT_MODE::GAME;
    PAD_INPUT_MODE shouldSendPadTo = PAD_INPUT_MODE::GAME;
    PAD_INPUT_MODE priorShouldSendPadTo = PAD_INPUT_MODE::GAME;
    
    //typing stuff
    bool textInputMode = false;
    char activeLine[MAX_INPUT_TEXT_LENGTH];
    unsigned activeLineCursor = 0;
    bool lineCommitted = false;
    
    void resetTextInputLine(){
        lineCommitted = false;
        activeLineCursor = 0;
        activeLine[0] = '\0';
    }
    
    // mouse stuff
    glm::vec2 windowSpaceToViewportSpace(glm::vec2 windowSpace);
    
    bool hasPendingClick = false;
    bool mouseDown = false;
    bool isDragging = false;
    
    glm::vec2 mouseDownPositionWindowSpace;
    glm::vec2 mouseDownPositionViewportSpace;
    glm::vec2 mouseDragPositionWindowSpace;
    glm::vec2 mouseDragPositionViewportSpace;
    glm::vec2 clickWindowSpace;
    glm::vec2 clickViewportSpace;
    
    ray getRaycast(glm::vec2 mouseViewportSpace);
    
    void resetClick(){
        hasPendingClick = false;
    }
    
    bool A      = false;
    bool B      = false;
    bool X      = false;
    bool Y      = false;
    bool start  = false;
    bool back   = false;
    bool RB     = false;
    bool LB     = false;
    bool RT     = false;
    bool LT     = false;
    bool LPush  = false;
    bool RPush  = false;
    bool DUp    = false;
    bool DDown  = false;
    bool DLeft  = false;
    bool DRight = false;
    
    float LStickX  = 0.0f;
    float LStickY  = 0.0f;
    float RStickX  = 0.0f;
    float RStickY  = 0.0f;
    float LTAnalog = 0.0f;
    float RTAnalog = 0.0f;
    
};

#endif /* InputSingleton_hpp */
