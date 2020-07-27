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
    
}

void EditorSystem::tick(float dt){
    InputSingleton& input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;
    CameraComponent& camC = m_admin.m_EditorSingleton.editorCameraComponent;
    
    
    if(input.DUp){
        camTransformC.m_position = camTransformC.m_position + (camC.m_forward * 0.2f);
    }
    if(input.DDown){
        camTransformC.m_position = camTransformC.m_position - (camC.m_forward * 0.2f);
    }
    if(input.DRight){
        camTransformC.m_position = camTransformC.m_position + (glm::cross(camC.m_forward, camC.m_up) * 0.2f);
    }
    if(input.DLeft){
        camTransformC.m_position = camTransformC.m_position - (glm::cross(camC.m_forward, camC.m_up) * 0.2f);
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
