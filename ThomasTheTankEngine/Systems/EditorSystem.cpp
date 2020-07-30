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

void EditorSystem::init(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    edit.defaultEditorCameraComponent = edit.editorCameraComponent;
    edit.defaultEditorCameraTransform = edit.editorCameraTransform;
}

const float keyboardEditorMovementSpeed = 0.2f;
const float controllerEditorMovementSpeed = 0.6f;

void EditorSystem::tick(uint64_t dt){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton& input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;
    CameraComponent& camC = m_admin.m_EditorSingleton.editorCameraComponent;
    
    if(input.shouldSendKeysTo == KEY_INPUT_MODE::EDITOR){
        //TODO: @Cleanup: abstract away the gainput stuff here with a function call
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
   
    //TODO: @Remove forced always true once I add player control
    if(input.shouldSendPadTo == PAD_INPUT_MODE::EDITOR or true){
        // Controller editor camera movement
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getRight() * input.LStickX * controllerEditorMovementSpeed);
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getForward() * input.LStickY * controllerEditorMovementSpeed);
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getUp() * input.LTAnalog * controllerEditorMovementSpeed);
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getUp() * input.RTAnalog * controllerEditorMovementSpeed);
    }
    
}

// pre: Imgui must be running
void EditorSystem::render(){
    ImGui::Begin("Editor");

    for (std::pair<entityID, Entity*> p : m_admin.m_entities){
        DebugNameComponent* nameC = m_admin.tryGetComponent<DebugNameComponent>(p.first);
        if(nameC != nullptr){
            ImGui::Text("%s", nameC->m_name.c_str());
        } else {
            ImGui::Text("%d", p.first);
        }
        TransformComponent* transC = m_admin.tryGetComponent<TransformComponent>(p.first);
        if(transC != nullptr){
            transC->imDisplay();
        }
    }
    
//    ImGui::InputVec3("Camera forward", &cameraC->m_forward);
    
    ImGui::End();
}
