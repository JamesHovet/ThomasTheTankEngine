//
//  EditorSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EditorSystem.hpp"
#include "allFamilies.h"
#include "imgui/imgui.h"
#include "ImGuiUtils.hpp"
#include "EntityAdmin.hpp"
#include "DebugNameComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "IntersectionUtils.hpp"

void EditorSystem::init(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    edit.defaultEditorCameraComponent = CameraComponent();
    edit.defaultEditorCameraTransform = TransformComponent();
    edit.defaultEditorCameraTransform.m_orientation = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    edit.editorCameraComponent = edit.defaultEditorCameraComponent;
    edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    edit.currentEditMode = EditMode::MOVE;
    
    initRendering();
}

const float keyboardEditorMovementSpeed = 0.2f;
const float controllerEditorMovementSpeed = 0.6f;

static void processEditorCameraKeyInput(TransformComponent &camTransformC, uint64_t dt, EditorSingleton &edit, InputSingleton &input) {
    if(input.rawSDLState[SDL_SCANCODE_R]){
        edit.editorCameraComponent = edit.defaultEditorCameraComponent;
        edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    }
    
    // Keyboard editor camera movement
    if(input.rawSDLState[SDL_SCANCODE_W]){
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getForward() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_S]){
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getForward() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_D]){
        camTransformC.m_position = camTransformC.m_position - camTransformC.getRight() * keyboardEditorMovementSpeed;
    }
    if(input.rawSDLState[SDL_SCANCODE_A]){
        camTransformC.m_position = camTransformC.m_position + camTransformC.getRight() * keyboardEditorMovementSpeed;
    }
    if(input.rawSDLState[SDL_SCANCODE_LSHIFT]){
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getUp() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_LCTRL]){
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getUp() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_Q]){
        camTransformC.m_orientation = glm::rotate(camTransformC.m_orientation, 1.0f * seconds(dt), camTransformC.getUp());
    }
    if(input.rawSDLState[SDL_SCANCODE_E]){
        camTransformC.m_orientation = glm::rotate(camTransformC.m_orientation, -1.0f * seconds(dt), camTransformC.getUp());
    }
}

static void processEditorCameraPadInput(TransformComponent &camTransformC, InputSingleton &input) {
    camTransformC.m_position = camTransformC.m_position - (camTransformC.getRight() * input.LStickX * controllerEditorMovementSpeed);
    camTransformC.m_position = camTransformC.m_position + (camTransformC.getForward() * input.LStickY * controllerEditorMovementSpeed);
    camTransformC.m_position = camTransformC.m_position - (camTransformC.getUp() * input.LTAnalog * controllerEditorMovementSpeed);
    camTransformC.m_position = camTransformC.m_position + (camTransformC.getUp() * input.RTAnalog * controllerEditorMovementSpeed);
}

void EditorSystem::tick(uint64_t dt){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton& input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;
    
    if(edit.hasSelectedEntity && m_admin.tryGetEntity(edit.selectedEntity) == nullptr){
        edit.hasSelectedEntity = false;
    }

    if(input.shouldSendKeysTo == KEY_INPUT_MODE::EDITOR){
        processEditorCameraKeyInput(camTransformC, dt, edit, input);
       
        // TODO: Use new drag stuff from the input system to drag objects in world space along axes
        if(edit.hasSelectedEntity){
            if(input.isDragging){
                TransformComponent* trans = m_admin.tryGetComponent<TransformComponent>(edit.selectedEntity);
                if(trans != nullptr){
                    if(! edit.isDraggingAxis){
                        switch (edit.currentEditMode){
                            case EditMode::MOVE:
                            case EditMode::SCALE:
                                edit.isDraggingAxis = getShouldDragMoveAxis(&edit.draggedAxis);
                                break;
                            case EditMode::ROTATE:
                                edit.isDraggingAxis = getShouldDragRotateAxis(&edit.draggedAxis);
                                break;
                        }
                        edit.selectedTransformCopyAtSelectionTime = *trans;
                        edit.draggedAxisWorldspace = getWorldspaceAxisToDrag(&edit.draggedAxisWorldspaceTangent, &edit.draggedAxisWorldspaceBinormal);
                    }
                    if(edit.isDraggingAxis){
                        //TODO: make this math actually correct
                        ray raycast0 = input.getRaycast(input.mouseDownPositionViewportSpace);
                        ray raycast1 = input.getRaycast(input.mouseDragPositionViewportSpace);
                        glm::vec3 delta = raycast1.dir - raycast0.dir;
                        glm::vec3 axis = edit.draggedAxisWorldspace;
                        glm::vec3 projected = axis * glm::dot(delta, axis);
                        if (edit.currentEditMode == EditMode::MOVE){
                            trans->m_position = trans->m_position + projected;
                        } else if (edit.currentEditMode == EditMode::SCALE){
//                            //TODO: MATH
//                            printf("s (%f, %f, %f) * (%f, %f, %f)\n", trans->m_scale.x, trans->m_scale.y, trans->m_scale.z, projected.x, projected.y, projected.z);
                            trans->m_scale = trans->m_scale * (glm::vec3(1.0) + glm::vec3(-1.0f, -1.0f, 1.0f) * projected);
                        } else if (edit.currentEditMode == EditMode::ROTATE){
                            Plane rotationPlane = {trans->m_position, axis};
                            
//                            if(glm::dot(rotationPlane.normal,(rotationPlane.origin - edit.editorCameraTransform.m_position)) < 0.0f){
//                                axis = -axis;
//                                printf("flip!: ");
//                            }
                            
                            glm::vec3 hit0;
                            Intersection::RayPlaneAbsolute(raycast0, rotationPlane, &hit0);
                            glm::vec3 hit1;
                            Intersection::RayPlaneAbsolute(raycast1, rotationPlane, &hit1);
                            
                            glm::vec3 dir0 = glm::normalize(hit0 - trans->m_position);
                            glm::vec3 dir1 = glm::normalize(hit1 - trans->m_position);
                            
                            float angle = glm::orientedAngle(dir0, dir1, axis);
                            printf("angle: %1.5f, axis: (%4.4f, %4.4f, %4.4f)\n", angle, axis.x, axis.y, axis.z);
                            
                            trans->m_orientation = glm::rotate(edit.selectedTransformCopyAtSelectionTime.m_orientation, angle, axis);
                        }
                    }
                }
                
            } else {
                edit.isDraggingAxis = false;
            }
        }
        
        if(input.hasPendingClick){
            ray r = input.getRaycast(input.clickViewportSpace);
            input.resetClick();
            entityID targetEID;
            glm::vec3 hit;
            if (getClosestOBBIntersectionEntity(r, &targetEID, &hit)){
                edit.hasSelectedEntity = true;
                edit.selectedEntity = targetEID;
            } else {
                edit.hasSelectedEntity = false;
            }
        }
    }
   
    //TODO: @Remove forced always true once I add player control
    if(input.shouldSendPadTo == PAD_INPUT_MODE::EDITOR || true){
        // Controller editor camera movement
        processEditorCameraPadInput(camTransformC, input);
    }
    
}


bool EditorSystem::getShouldDragMoveAxis(AXIS* axisToDrag){
    InputSingleton& input = m_admin.m_InputSingleton;
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    TransformComponent& selectedTransform = m_admin.getComponent<TransformComponent>(edit.selectedEntity);
    
    ray r = input.getRaycast(input.mouseDownPositionViewportSpace);
    Cylinder collider = {glm::vec3(0.0), glm::vec3(1.5f, 0.0f, 0.0f), 0.1f};
    glm::mat4 baseMatrix;
    
    if(edit.usingLocalWorldSpace){
        baseMatrix = selectedTransform.getMat4Unscaled();
    } else {
        baseMatrix = glm::translate(selectedTransform.m_position);
    }
    
    if(Intersection::RayCyl(r, collider, baseMatrix)){
        *axisToDrag = AXIS::X;
        return true;
    }
    glm::mat4 matY = glm::rotate(baseMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    if(Intersection::RayCyl(r, collider, matY)){
        *axisToDrag = AXIS::Y;
        return true;
    }
    glm::mat4 matZ = glm::rotate(baseMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if(Intersection::RayCyl(r, collider, matZ)){
        *axisToDrag = AXIS::Z;
        return true;
    }
    
    return false;
}

bool EditorSystem::getShouldDragRotateAxis(AXIS* axisToDrag){
    InputSingleton& input = m_admin.m_InputSingleton;
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    TransformComponent& selectedTransform = m_admin.getComponent<TransformComponent>(edit.selectedEntity);
    
    ray r = input.getRaycast(input.mouseDownPositionViewportSpace);
    
    //Early out -- can't intersect the cylinder colliders without intersecting the sphere (within a small margin of error)
    float dummyf;
    if(! glm::intersectRaySphere(r.orig, r.dir, selectedTransform.m_position, 1, dummyf)){
        return false;
    }
    
    const float half_width = 0.15f;
    Cylinder collider = {glm::vec3(-half_width, 0.0f, 0.0f), glm::vec3(half_width, 0.0f, 0.0f), 1.0f};
    glm::mat4 baseMatrix;
    
    if(edit.usingLocalWorldSpace){
        baseMatrix = selectedTransform.getMat4Unscaled();
    } else {
        baseMatrix = glm::translate(selectedTransform.m_position);
    }
    
    bool didHit = false;
    const float threshold = 0.8f;
    
    glm::vec3 hitX;
    glm::vec3 hitY;
    glm::vec3 hitZ;
    
    float distanceFromCenterX = 0.0f;
    float distanceFromCenterY = 0.0f;
    float distanceFromCenterZ = 0.0f;
    
    bool didHitX = Intersection::RayCyl(r, collider, baseMatrix, &hitX);
    bool didHitY = Intersection::RayCyl(r, collider, glm::rotate(baseMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), &hitY);
    bool didHitZ = Intersection::RayCyl(r, collider, glm::rotate(baseMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), &hitZ);
    
    if(didHitX){
        distanceFromCenterX = glm::distance(selectedTransform.m_position, hitX);
        if(distanceFromCenterX > threshold){
            didHit = true;
            *axisToDrag = AXIS::X;
        }
    }
    if (didHitY) {
        distanceFromCenterY = glm::distance(selectedTransform.m_position, hitY);
        if(distanceFromCenterY > threshold && distanceFromCenterY > distanceFromCenterX){
            didHit = true;
            *axisToDrag = AXIS::Y;
        }
    }
    if(didHitZ){
        distanceFromCenterZ = glm::distance(selectedTransform.m_position, hitZ);
        if(distanceFromCenterZ > threshold && distanceFromCenterZ > distanceFromCenterX && distanceFromCenterZ > distanceFromCenterY){
            didHit = true;
            *axisToDrag = AXIS::Z;
        }
    }
    
    return didHit;
}

glm::vec3 EditorSystem::getWorldspaceAxisToDrag(glm::vec3* tangent, glm::vec3* binormal){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    TransformComponent& trans = m_admin.getComponent<TransformComponent>(edit.selectedEntity);
    glm::vec3 out;
    switch (edit.currentEditMode) {
        case EditMode::MOVE:
            if(edit.usingLocalWorldSpace){
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out       = trans.getRight();
                        *tangent  = trans.getUp();
                        *binormal = trans.getForward();
                        break;
                    case AXIS::Y:
                        out       = trans.getUp();
                        *tangent  = trans.getForward();
                        *binormal = trans.getRight();
                        break;
                    case AXIS::Z:
                        out       = trans.getForward();
                        *tangent  = trans.getRight();
                        *binormal = trans.getForward();
                        break;
                }
            } else {
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out       = glm::vec3(1.0f, 0.0f, 0.0f);
                        *tangent  = glm::vec3(0.0f, 1.0f, 0.0f);
                        *binormal = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                    case AXIS::Y:
                        out       = glm::vec3(0.0f, 1.0f, 0.0f);
                        *tangent  = glm::vec3(0.0f, 0.0f, 1.0f);
                        *binormal = glm::vec3(1.0f, 0.0f, 0.0f);
                        break;
                    case AXIS::Z:
                        out       = glm::vec3(0.0f, 0.0f, 1.0f);
                        *tangent  = glm::vec3(1.0f, 0.0f, 0.0f);
                        *binormal = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                }
            }
            break;
           
        case EditMode::SCALE:
            if(edit.usingLocalWorldSpace){
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out       = glm::vec3(1.0f, 0.0f, 0.0f);
                        *tangent  = glm::vec3(0.0f, 1.0f, 0.0f);
                        *binormal = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                    case AXIS::Y:
                        out       = glm::vec3(0.0f, 1.0f, 0.0f);
                        *tangent  = glm::vec3(0.0f, 0.0f, 1.0f);
                        *binormal = glm::vec3(1.0f, 0.0f, 0.0f);
                        break;
                    case AXIS::Z:
                        out       = glm::vec3(0.0f, 0.0f, 1.0f);
                        *tangent  = glm::vec3(1.0f, 0.0f, 0.0f);
                        *binormal = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                }
            } else {
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out       = trans.getRight();
                        *tangent  = trans.getUp();
                        *binormal = trans.getForward();
                        break;
                    case AXIS::Y:
                        out       = trans.getUp();
                        *tangent  = trans.getForward();
                        *binormal = trans.getRight();
                        break;
                    case AXIS::Z:
                        out       = trans.getForward();
                        *tangent  = trans.getRight();
                        *binormal = trans.getForward();
                        break;
                }
            }
            break;
        case EditMode::ROTATE:
            if(edit.usingLocalWorldSpace){
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out       = glm::vec3(1.0f, 0.0f, 0.0f);
                        *tangent  = glm::vec3(0.0f, 1.0f, 0.0f);
                        *binormal = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                    case AXIS::Y:
                        out       = glm::vec3(0.0f, 1.0f, 0.0f);
                        *tangent  = glm::vec3(0.0f, 0.0f, 1.0f);
                        *binormal = glm::vec3(1.0f, 0.0f, 0.0f);
                        break;
                    case AXIS::Z:
                        out       = glm::vec3(0.0f, 0.0f, 1.0f);
                        *tangent  = glm::vec3(1.0f, 0.0f, 0.0f);
                        *binormal = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                }
            } else {
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out       = trans.getRight();
                        *tangent  = trans.getUp();
                        *binormal = trans.getForward();
                        break;
                    case AXIS::Y:
                        out       = trans.getUp();
                        *tangent  = trans.getForward();
                        *binormal = trans.getRight();
                        break;
                    case AXIS::Z:
                        out       = trans.getForward();
                        *tangent  = trans.getRight();
                        *binormal = trans.getForward();
                        break;
                }
                
            }
            break;
    }
    return out;
}

bool EditorSystem::getClosestOBBIntersectionEntity(ray r, entityID *eID, glm::vec3 *hitOutput){
    float closestD = INFINITY;
    entityID closest = -1;
    glm::vec3 hit;
    
    for(std::pair<entityID, AABBCollisionFamily> p : m_admin.getFamilyMap<AABBCollisionFamily>()){
        AABBCollisionFamily f = p.second;
        AABB box = f.m_AABBColliderComponent.m_AABB;
        glm::mat4 model = f.m_TransformComponent.getLocalModelMatrix();
        glm::vec3 thisHit;
        float d;
        bool didIntersect = Intersection::RayOBB(r, box, model, &d, &thisHit);
        if(didIntersect){
            if(d < closestD){
                closest = p.first;
                closestD = d;
                hit = thisHit;
            }
        }
    }
    
    if(closest != -1){
        *eID = closest;
        *hitOutput = hit;
        return true;
    }
    return false;
}
