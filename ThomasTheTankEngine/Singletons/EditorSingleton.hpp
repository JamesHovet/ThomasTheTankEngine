//
//  EditorSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef EditorSingleton_h
#define EditorSingleton_h

#include "CameraFamily.hpp"

struct EditorSingleton {
    bool shouldUseEditorCamera {false};
    
    TransformComponent editorCameraTransform;
    CameraComponent    editorCameraComponent;
    CameraFamily       editorCameraFamily{0, editorCameraTransform, editorCameraComponent};
    
};

#endif /* EditorSingleton_h */
