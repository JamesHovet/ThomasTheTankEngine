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

enum class EditMode {
    MOVE,
    SCALE,
    ROTATE
};

struct EditorSingleton {
    bool shouldUseEditorCamera {false};
    bool shouldTakeEditorInput {false};
    
    TransformComponent editorCameraTransform;
    CameraComponent    editorCameraComponent;
    
    TransformComponent defaultEditorCameraTransform;
    CameraComponent    defaultEditorCameraComponent;
    
    bool hasSelectedEntity;
    entityID selectedEntity;
    TransformComponent selectedTransformCopyAtSelectionTime;
    
    EditMode currentEditMode {EditMode::MOVE};
    
    bool usingLocalWorldSpace {true};
    
    bool isDraggingAxis {false};
    AXIS draggedAxis;
    glm::vec3 draggedAxisWorldspace;
    glm::vec3 draggedAxisWorldspaceTangent;
    glm::vec3 draggedAxisWorldspaceBinormal;
    
};

#endif /* EditorSingleton_h */
