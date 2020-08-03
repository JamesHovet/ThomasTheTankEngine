//
//  EditorSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EditorSystem.hpp"
#include "libs/imgui/imgui.h"
#include "ImGuiUtils.hpp"
#include "EntityAdmin.hpp"
#include "DebugNameComponent.hpp"

#include "IntersectionUtils.hpp"

void EditorSystem::init(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    edit.defaultEditorCameraComponent = edit.editorCameraComponent;
    edit.defaultEditorCameraTransform = edit.editorCameraTransform;
}

const float keyboardEditorMovementSpeed = 0.2f;
const float controllerEditorMovementSpeed = 0.6f;

static void processEditorCameraKeyInput(TransformComponent &camTransformC, uint64_t dt, EditorSingleton &edit, InputSingleton &input) {
    if(input.keyboard->GetBool(gainput::KeyR)){ // reset camera
        edit.editorCameraComponent = edit.defaultEditorCameraComponent;
        edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    }
    
    
    // Keyboard editor camera movement
    if(input.keyboard->GetBool(gainput::KeyW)){
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getForward() * keyboardEditorMovementSpeed);
    }
    if(input.keyboard->GetBool(gainput::KeyS)){
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getForward() * keyboardEditorMovementSpeed);
    }
    if(input.keyboard->GetBool(gainput::KeyD)){
        camTransformC.m_position = camTransformC.m_position - camTransformC.getRight() * keyboardEditorMovementSpeed;
    }
    if(input.keyboard->GetBool(gainput::KeyA)){
        camTransformC.m_position = camTransformC.m_position + camTransformC.getRight() * keyboardEditorMovementSpeed;
    }
    if(input.keyboard->GetBool(gainput::KeyShiftL)){
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getUp() * keyboardEditorMovementSpeed);
    }
    if(input.keyboard->GetBool(gainput::KeyCtrlL)){
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getUp() * keyboardEditorMovementSpeed);
    }
    if(input.keyboard->GetBool(gainput::KeyQ)){
        camTransformC.m_orientation = glm::rotate(camTransformC.m_orientation, 1.0f * seconds(dt), camTransformC.getUp());
    }
    if(input.keyboard->GetBool(gainput::KeyE)){
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
    CameraComponent& camC = m_admin.m_EditorSingleton.editorCameraComponent;
    
    if(input.shouldSendKeysTo == KEY_INPUT_MODE::EDITOR){
        //TODO: @Cleanup: abstract away the gainput stuff here with a function call
        processEditorCameraKeyInput(camTransformC, dt, edit, input);
        
        if(input.hasPendingClick){
            ray r;
            glm::vec4 mouse = glm::vec4(input.clickViewportSpace.x, input.clickViewportSpace.y, -1.0f, 1.0f);
//            glm::vec4 pos = mouse * m_admin.m_RenderSingleton.screenToWorld();
            glm::vec4 pos = glm::inverse(m_admin.m_RenderSingleton.projection * m_admin.m_RenderSingleton.view) * mouse;
//
            pos.x /= pos.w;
            pos.y /= pos.w;
            pos.z /= pos.w;
            
            r.orig = m_admin.m_EditorSingleton.editorCameraTransform.m_position;
            r.dir = glm::normalize(glm::vec3(pos.x, pos.y, pos.z) - r.orig);
            
//            printf("%f, %f, %f\n", r.dir.x, r.dir.y, r.dir.z);
            
//            printf("%f, %f, %f, %f\n", mouse.x, mouse.y, mouse.z, mouse.w);
            input.resetClick();
            
            float closestD = INFINITY;
            entityID closest = -1;
            
            for(std::pair<entityID, AABBCollisionFamily> p : m_admin.getFamilyMap<AABBCollisionFamily>()){
                AABBCollisionFamily f = p.second;
                AABB box = f.m_AABBColliderComponent.m_AABB;
                box = {
                    box.min + f.m_TransformComponent.m_position,
                    box.max + f.m_TransformComponent.m_position
                };
                
                
                float d;
                bool didIntersect = Intersection::RayAABB(r, box, &d);
                if(didIntersect){
                    if(d < closestD){
                        closest = p.first;
                        closestD = d;
                    }
                }
            }
            
            if(closest != -1){
                GreyBoxComponent * box = m_admin.tryGetComponent<GreyBoxComponent>(closest);
                if(box != nullptr){
                    box->m_color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
                }
            }
            
        }
    }
   
    //TODO: @Remove forced always true once I add player control
    if(input.shouldSendPadTo == PAD_INPUT_MODE::EDITOR or true){
        // Controller editor camera movement
        processEditorCameraPadInput(camTransformC, input);
    }
    
}

// pre: Imgui must be running
void EditorSystem::render(){
    ImGui::Begin("Editor");

    char nameBuf[32];
    
    for (std::pair<entityID, Entity*> p : m_admin.m_entities){
        entityID eID = p.first;
        DebugNameComponent* nameC = m_admin.tryGetComponent<DebugNameComponent>(eID);
        
        if(nameC != nullptr){
            snprintf(nameBuf, 32, "%s", nameC->m_name.c_str());
        } else {
            snprintf(nameBuf, 32, "%d", eID);
        }
        
        ImGui::PushID(p.second);
        
        if(ImGui::TreeNode(nameBuf)){
            for(auto it = m_admin.componentsBegin(eID); it != m_admin.componentsEnd(eID); ++it){
                (*it)->imDisplay();
            }
            
            ImGui::TreePop();
        }
        
        ImGui::PopID();
    }
    
//    ImGui::InputVec3("Camera forward", &cameraC->m_forward);
    
    ImGui::End();
}
