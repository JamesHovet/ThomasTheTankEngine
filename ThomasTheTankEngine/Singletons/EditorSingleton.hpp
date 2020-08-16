//
//  EditorSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef EditorSingleton_h
#define EditorSingleton_h

#include "CameraFamily.hpp"

struct EditorSingleton {
    bool shouldUseEditorCamera {false};
    bool shouldTakeEditorInput {false};
    
    TransformComponent editorCameraTransform;
    CameraComponent    editorCameraComponent;
    
    TransformComponent defaultEditorCameraTransform;
    CameraComponent    defaultEditorCameraComponent;
    
    bool hasSelectedEntity;
    entityID selectedEntity;
    
    bool usingLocalWorldSpace {true};
    
    
    
};

#endif /* EditorSingleton_h */
