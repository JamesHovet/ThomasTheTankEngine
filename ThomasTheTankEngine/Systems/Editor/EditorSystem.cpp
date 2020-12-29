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

#include <algorithm>

#include "IntersectionUtils.hpp"

void EditorSystem::init(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    edit.defaultEditorCameraComponent = CameraComponent();
    edit.defaultEditorCameraTransform = TransformComponent();
    edit.defaultEditorCameraTransform.setLocalOrientation(glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    edit.editorCameraComponent = edit.defaultEditorCameraComponent;
    edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    edit.currentEditMode = EditMode::MOVE;
    
    initRendering();
}

const float keyboardEditorMovementSpeed = 0.2f;
const float controllerEditorMovementSpeed = 0.6f;

void EditorSystem::processEditorCameraKeyInput() {
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;
    TimeSingleton time = m_admin.m_TimeSingleton;
    if(input.rawSDLState[SDL_SCANCODE_R]){
        edit.editorCameraComponent = edit.defaultEditorCameraComponent;
        edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    }
    
    // Keyboard editor camera movement
    if(input.rawSDLState[SDL_SCANCODE_W]){
        camTransformC.setLocalPosition(camTransformC.getPosition() + (camTransformC.getForward() * keyboardEditorMovementSpeed));
    }
    if(input.rawSDLState[SDL_SCANCODE_S]){
        camTransformC.setLocalPosition(camTransformC.getPosition() - (camTransformC.getForward() * keyboardEditorMovementSpeed));
    }
    if(input.rawSDLState[SDL_SCANCODE_D]){
        camTransformC.setLocalPosition(camTransformC.getPosition() - camTransformC.getRight() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_A]){
        camTransformC.setLocalPosition(camTransformC.getPosition() + camTransformC.getRight() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_SPACE]){
        camTransformC.setLocalPosition(camTransformC.getPosition() + (camTransformC.getUp() * keyboardEditorMovementSpeed));
    }
    if(input.rawSDLState[SDL_SCANCODE_LCTRL]){
        camTransformC.setLocalPosition(camTransformC.getPosition() - (camTransformC.getUp() * keyboardEditorMovementSpeed));
    }
    if(input.rawSDLState[SDL_SCANCODE_Q]){
        camTransformC.setLocalOrientation(glm::rotate(camTransformC.getLocalOrientation(), 1.0f * time.fseconds, camTransformC.getUp3()));
    }
    if(input.rawSDLState[SDL_SCANCODE_E]){
        camTransformC.setLocalOrientation(glm::rotate(camTransformC.getLocalOrientation(), -1.0f * time.fseconds, camTransformC.getUp3()));
    }
}

void EditorSystem::processMouseDragForSingleSelection(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton& input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
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
            edit.draggedAxisLocal = getLocalAxisToDrag(&edit.draggedAxisLocalTangent, &edit.draggedAxisLocalBinormal);
        }
        if(edit.isDraggingAxis){
            //TODO: Make global stuff work again
            glm::vec4 pos = trans->getPosition();
            ray raycast0 = input.getRaycast(input.mouseDownPositionViewportSpace);
            ray raycast1 = input.getRaycast(input.mouseDragPositionViewportSpace);
            glm::vec4 delta = raycast1.dir - raycast0.dir;
            //@Temporary
            //@Temporary
            //@Temporary
            glm::vec3 origin = glm::vec3(0.3f, 0.3f, 0.0f);
            imm.drawLine2d(glm::vec3(origin), origin + glm::vec3(raycast1.dir) * 0.5f, RGBA_Red);
            imm.drawLine2d(glm::vec3(origin), origin + glm::vec3(raycast0.dir) * 0.5f, RGBA_Blue);
            imm.drawLine2d(origin + glm::vec3(raycast1.dir) * 0.5f, origin + glm::vec3(raycast0.dir) * 0.5f, RGBA_Green);
            glm::vec4 axis = edit.draggedAxisLocal;
            glm::vec4 axisTransformed = glm::normalize(edit.selectedTransformCopyAtSelectionTime.getMat4Unscaled() * axis);
            
            if (edit.currentEditMode == EditMode::MOVE){
                if(!edit.usingLocalWorldSpace){
                    axisTransformed = axis;
                }
                glm::vec4 projectedMove = axisTransformed * glm::dot(delta, axisTransformed);
                imm.drawLine2d(glm::vec3(origin), origin + glm::vec3(projectedMove) * 0.5f, RGBA_Black);
                trans->setLocalPosition(trans->getLocalPosition() + projectedMove);
            } else if (edit.currentEditMode == EditMode::SCALE){
                glm::vec4 projectedScale = axis * glm::dot(delta, axisTransformed);
                if(!edit.usingLocalWorldSpace){
                    projectedScale = axisTransformed * glm::dot(delta, axis);
                }
                trans->setScale(trans->getLocalScale() * (glm::vec4(1.0f, 1.0f, 1.0f, 0.0f) + projectedScale));
            } else if (edit.currentEditMode == EditMode::ROTATE){

                Plane rotationPlane = {trans->getPosition(), axisTransformed};
                if(!edit.usingLocalWorldSpace){
                    rotationPlane.normal = axis;
//                                glm::mat4 inverse = glm::inverse(edit.selectedTransformCopyAtSelectionTime.getMat4Unscaled());
//                                axis = axis * inverse;
                }
                
                glm::vec4 hit0;
                Intersection::RayPlaneAbsolute(raycast0, rotationPlane, &hit0);
                glm::vec4 hit1;
                Intersection::RayPlaneAbsolute(raycast1, rotationPlane, &hit1);
                
                glm::vec4 planeTangent = glm::normalize(hit0 - trans->getPosition());
                glm::vec4 planeBitangent = glm::vec4(glm::cross(glm::vec3(planeTangent), glm::vec3(rotationPlane.normal)), 0.0f);

                glm::vec4 dir0 = glm::normalize(hit0 - trans->getPosition());
                glm::vec4 dir1 = glm::normalize(hit1 - trans->getPosition());
                
                float angle = glm::orientedAngle(glm::vec3(dir0), glm::vec3(dir1), glm::vec3(rotationPlane.normal));
                float originalAxisToTransformedAxisDot = glm::dot(rotationPlane.normal, axisTransformed);
                if (originalAxisToTransformedAxisDot < 0){
                    angle = - angle;
                }
                
                trans->setLocalOrientation(glm::rotate(edit.selectedTransformCopyAtSelectionTime.getLocalOrientation(), angle, glm::vec3(axis)));
//                            printf("angle: %1.5f, axis: (%4.4f, %4.4f, %4.4f)\n", angle, axis.x, axis.y, axis.z);
                
                
                imm.drawTri3d(pos, pos + planeTangent * 100.0f, pos + planeBitangent * 100.0f, RGBA(0.5f, 0.5f, 0.5f, 0.3f));
                imm.drawTri3d(pos, pos + planeTangent * 100.0f, pos - planeBitangent * 100.0f, RGBA(0.5f, 0.5f, 0.5f, 0.3f));
                imm.drawTri3d(pos, pos - planeTangent * 100.0f, pos + planeBitangent * 100.0f, RGBA(0.5f, 0.5f, 0.5f, 0.3f));
                imm.drawTri3d(pos, pos - planeTangent * 100.0f, pos - planeBitangent * 100.0f, RGBA(0.5f, 0.5f, 0.5f, 0.3f));

                imm.drawLine3d(pos, pos + rotationPlane.normal * 5.0f, RGBA(1.0f, 0.0f, 1.0f, 1.0f));

                imm.drawLine3d(hit0, hit1, RGBA(1.0f, 1.0f, 1.0f, 1.0f));
                imm.drawLine3d(pos, pos + dir0 * 5.0f, RGBA_White);
                imm.drawLine3d(pos, pos + dir1 * 5.0f, RGBA_White);
                
            }
        }
    }
}

void EditorSystem::processMouseDragForMultiSelection(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton& input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;

    if(!edit.isDraggingAxis){
        switch (edit.currentEditMode){
            case EditMode::MOVE:
            case EditMode::SCALE:
                edit.isDraggingAxis = getShouldDragMoveAxis(&edit.draggedAxis);
                break;
            case EditMode::ROTATE:
                edit.isDraggingAxis = getShouldDragRotateAxis(&edit.draggedAxis);
                break;
        }
        edit.draggedAxisLocal = getLocalAxisToDrag(&edit.draggedAxisLocalTangent, &edit.draggedAxisLocalBinormal);
    }
    if(edit.isDraggingAxis){
        //TODO: Make global stuff work again
        ray raycast0 = input.getRaycast(input.mouseDownPositionViewportSpace);
        ray raycast1 = input.getRaycast(input.mouseDragPositionViewportSpace);
        glm::vec4 delta = raycast1.dir - raycast0.dir;
        //@Temporary
        //@Temporary
        //@Temporary
        
        glm::vec3 origin = glm::vec3(0.3f, 0.3f, 0.0f);
        imm.drawLine2d(glm::vec3(origin), origin + glm::vec3(raycast1.dir) * 0.5f, RGBA_Red);
        imm.drawLine2d(glm::vec3(origin), origin + glm::vec3(raycast0.dir) * 0.5f, RGBA_Blue);
        imm.drawLine2d(origin + glm::vec3(raycast1.dir) * 0.5f, origin + glm::vec3(raycast0.dir) * 0.5f, RGBA_Green);
    
        glm::vec4 axis = edit.draggedAxisLocal;
        
        if (edit.currentEditMode == EditMode::MOVE){
            glm::vec4 projectedMove = axis * glm::dot(delta, axis);
            imm.drawLine2d(glm::vec3(origin), origin + glm::vec3(projectedMove) * 0.5f, RGBA_Black);
            edit.multiselectionCenter.setLocalPosition(edit.multiselectionCenter.getLocalPosition() + projectedMove);
            for(entityID eID : edit.multiselectionEntities){
                TransformComponent* trans = m_admin.tryGetComponent<TransformComponent>(eID);
                if(trans){
                    trans->setLocalPosition(trans->getLocalPosition() + projectedMove);
                }
            }
        }
    }
}

void EditorSystem::startMultiselection(entityID eID, entityID other){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    edit.multiselectionEntities.clear();
    edit.multiselectionEntities.push_back(eID);
    glm::vec4 selectedPosition =  m_admin.getComponent<TransformComponent>(eID).getPosition();
    edit.multiselectionCenter.setLocalPosition(selectedPosition);
    addToMultiselection(other);
}

void EditorSystem::addToMultiselection(entityID eID){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    glm::vec4 targetPos = m_admin.getComponent<TransformComponent>(eID).getPosition();
    float numSelected = (float) edit.multiselectionEntities.size();
    glm::vec4 oldCenter = edit.multiselectionCenter.getPosition();
    glm::vec4 newCenter = ((oldCenter * numSelected) + targetPos) / (numSelected + 1.0f);
    edit.multiselectionCenter.setLocalPosition(newCenter);
    edit.multiselectionCenterCopyAtSelectionTime = edit.multiselectionCenter;
    edit.multiselectionEntities.push_back(eID);
}

void EditorSystem::processClick(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton& input = m_admin.m_InputSingleton;
    ray r = input.getRaycast(input.clickViewportSpace);
    input.resetClick();
    entityID targetEID;
    glm::vec4 hit;
    bool didHit = getClosestOBBIntersectionEntity(r, &targetEID, &hit);
    
    if(didHit){
        if(edit.hasMultiselection && input.shift){
            addToMultiselection(targetEID);
            return;
        }
        
        if(edit.hasSelectedEntity && input.shift){
            edit.hasSelectedEntity = false;
            edit.hasMultiselection = true;
            startMultiselection(edit.selectedEntity, targetEID);
            return;
        }
        
        edit.hasMultiselection = false;
        edit.hasSelectedEntity = true;
        edit.selectedEntity = targetEID;
    } else {
        edit.hasSelectedEntity = false;
        edit.hasMultiselection = false;
    }
}

void EditorSystem::ensureSelectionNotDeleted(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    if(edit.hasSelectedEntity && !m_admin.entityExists(edit.selectedEntity)){
        edit.hasSelectedEntity = false;
    }
    if(edit.hasMultiselection){
        for(entityID eID : edit.multiselectionEntities){
            if(!m_admin.entityExists(eID)){
                edit.hasMultiselection = false;
                edit.multiselectionEntities.clear();
                return;
            }
        }
    }
}

void EditorSystem::tick(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton input = m_admin.m_InputSingleton;
    
    ensureSelectionNotDeleted();

    if(input.shouldSendKeysTo == KEY_INPUT_MODE::EDITOR){
        processEditorCameraKeyInput();
       
        if(input.isDragging){
            if(edit.hasSelectedEntity){processMouseDragForSingleSelection();}
            if(edit.hasMultiselection){processMouseDragForMultiSelection();}
        } else {
            edit.isDraggingAxis = false;
        }
        
        if(input.hasPendingClick){
            processClick();
        }
    }
}

TransformComponent& EditorSystem::getGizmoTransform(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    if(edit.hasMultiselection){
        return edit.multiselectionCenter;
    } else {
        return m_admin.getComponent<TransformComponent>(edit.selectedEntity);
    }
}

bool EditorSystem::getShouldDragMoveAxis(AXIS* axisToDrag){
    InputSingleton& input = m_admin.m_InputSingleton;
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    TransformComponent& selectedTransform = getGizmoTransform();
    
    ray r = input.getRaycast(input.mouseDownPositionViewportSpace);
    Cylinder collider = {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.5f, 0.0f, 0.0f, 1.0f), 0.1f};
    glm::mat4 baseMatrix;
    
    if(edit.usingLocalWorldSpace){
        baseMatrix = selectedTransform.getMat4Unscaled();
    } else {
        baseMatrix = glm::translate(selectedTransform.getPosition3());
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
    TransformComponent& selectedTransform = getGizmoTransform();
    
    ray r = input.getRaycast(input.mouseDownPositionViewportSpace);
    
    //Early out -- can't intersect the cylinder colliders without intersecting the sphere (within a small margin of error)
    float dummyf;
    if(! glm::intersectRaySphere(r.orig, r.dir, selectedTransform.getPosition(), 1, dummyf)){
        return false;
    }
    
    const float half_width = 0.15f;
    Cylinder collider = {glm::vec4(-half_width, 0.0f, 0.0f, 1.0f), glm::vec4(half_width, 0.0f, 0.0f, 1.0f), 1.0f};
    glm::mat4 baseMatrix;
    
    if(edit.usingLocalWorldSpace){
        baseMatrix = selectedTransform.getMat4Unscaled();
    } else {
        baseMatrix = glm::translate(selectedTransform.getPosition3());
    }
    
    bool didHit = false;
    const float threshold = 0.8f;
    
    glm::vec4 hitX;
    glm::vec4 hitY;
    glm::vec4 hitZ;
    
    float distanceFromCenterX = 0.0f;
    float distanceFromCenterY = 0.0f;
    float distanceFromCenterZ = 0.0f;
    
    bool didHitX = Intersection::RayCyl(r, collider, baseMatrix, &hitX);
    bool didHitY = Intersection::RayCyl(r, collider, glm::rotate(baseMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), &hitY);
    bool didHitZ = Intersection::RayCyl(r, collider, glm::rotate(baseMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), &hitZ);
    
    if(didHitX){
        distanceFromCenterX = glm::distance(selectedTransform.getPosition(), hitX);
        if(distanceFromCenterX > threshold){
            didHit = true;
            *axisToDrag = AXIS::X;
        }
    }
    if (didHitY) {
        distanceFromCenterY = glm::distance(selectedTransform.getPosition(), hitY);
        if(distanceFromCenterY > threshold && distanceFromCenterY > distanceFromCenterX){
            didHit = true;
            *axisToDrag = AXIS::Y;
        }
    }
    if(didHitZ){
        distanceFromCenterZ = glm::distance(selectedTransform.getPosition(), hitZ);
        if(distanceFromCenterZ > threshold && distanceFromCenterZ > distanceFromCenterX && distanceFromCenterZ > distanceFromCenterY){
            didHit = true;
            *axisToDrag = AXIS::Z;
        }
    }
    
    return didHit;
}

glm::vec4 EditorSystem::getLocalAxisToDrag(glm::vec4* tangent, glm::vec4* binormal){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    glm::vec4 out;
    switch (edit.draggedAxis) {
        case AXIS::X:
            out       = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
            *tangent  = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
            *binormal = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
            break;
        case AXIS::Y:
            out       = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
            *tangent  = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
            *binormal = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
            break;
        case AXIS::Z:
            out       = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
            *tangent  = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
            *binormal = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
            break;
    }
    return out;
}

bool EditorSystem::getClosestOBBIntersectionEntity(ray r, entityID *eID, glm::vec4 *hitOutput){
    float closestD = INFINITY;
    entityID closest = NO_ENTITY;
    glm::vec4 hit;
    
    std::vector<AABBCollisionFamilyStatic> staticAABBFamilies = m_admin.getFamilyStaticVector<AABBCollisionFamilyStatic>();
    for(AABBCollisionFamilyStatic f : staticAABBFamilies){
        AABB box = f.m_AABBColliderComponent.m_AABB;
        glm::mat4 model = f.m_TransformComponent.m_cachedMat4;
        glm::vec4 thisHit;
        float d;
        bool didIntersect = Intersection::RayOBB(r, box, model, &d, &thisHit);
        if(didIntersect){
            if(d < closestD){
                closest = f.eID;
                closestD = d;
                hit = thisHit;
            }
        }
    }
    
    if(closest == NO_ENTITY) return false;
    
    *eID = closest;
    *hitOutput = hit;
    return true;
}
