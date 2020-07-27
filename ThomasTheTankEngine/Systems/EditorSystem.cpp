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

void EditorSystem::tick(float dt){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton& input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;
    CameraComponent& camC = m_admin.m_EditorSingleton.editorCameraComponent;
    if(edit.shouldUseEditorCamera){
        //TODO: @Cleanup: abstract away the gainput stuff here with a function call
        if(input.keyboard->GetBool(gainput::KeyR)){ // reset camera
            edit.editorCameraComponent = edit.defaultEditorCameraComponent;
            edit.editorCameraTransform = edit.defaultEditorCameraTransform;
        }
        
        glm::vec3 right = (glm::cross(camC.m_forward, camC.m_up));
        // Keyboard editor camera movement
        if(input.keyboard->GetBool(gainput::KeyW)){
            camTransformC.m_position = camTransformC.m_position + (camC.m_forward * keyboardEditorMovementSpeed);
        }
        if(input.keyboard->GetBool(gainput::KeyS)){
            camTransformC.m_position = camTransformC.m_position - (camC.m_forward * keyboardEditorMovementSpeed);
        }
        if(input.keyboard->GetBool(gainput::KeyD)){
            camTransformC.m_position = camTransformC.m_position + right * keyboardEditorMovementSpeed;
        }
        if(input.keyboard->GetBool(gainput::KeyA)){
            camTransformC.m_position = camTransformC.m_position - right * keyboardEditorMovementSpeed;
        }
        if(input.keyboard->GetBool(gainput::KeyShiftL)){
            camTransformC.m_position = camTransformC.m_position + (camC.m_up * keyboardEditorMovementSpeed);
        }
        if(input.keyboard->GetBool(gainput::KeyCtrlL)){
            camTransformC.m_position = camTransformC.m_position - (camC.m_up * keyboardEditorMovementSpeed);
        }
        
        // Controller editor camera movement
        camTransformC.m_position = camTransformC.m_position + (right * input.LStickX * controllerEditorMovementSpeed);
        camTransformC.m_position = camTransformC.m_position + (camC.m_forward * input.LStickY * controllerEditorMovementSpeed);
        std::cout << "---" << std::endl;
        std::cout << input.LTAnalog << std::endl;
        std::cout << input.RTAnalog << std::endl;
        camTransformC.m_position = camTransformC.m_position + (camC.m_up * input.LTAnalog * controllerEditorMovementSpeed);
        camTransformC.m_position = camTransformC.m_position - (camC.m_up * input.RTAnalog * controllerEditorMovementSpeed);

        
    }
    
}

// pre: Imgui must be running
void EditorSystem::render(){
    ImGui::Begin("Editor");
    
    CameraComponent* cameraC = nullptr;
    
    for (std::pair<entityID, Entity*> p : m_admin.m_entities){
        DebugNameComponent* nameC = m_admin.tryGetComponent<DebugNameComponent>(p.first);
        if(nameC == nullptr){continue;}
        ImGui::Text("%s", nameC->m_name.c_str());
        
        CameraComponent* camC = m_admin.tryGetComponent<CameraComponent>(p.first);
        if(camC != nullptr){
            cameraC = camC;
        }
    }
    
    ImGui::InputVec3("Camera forward", &cameraC->m_forward);
    
    ImGui::End();
}
