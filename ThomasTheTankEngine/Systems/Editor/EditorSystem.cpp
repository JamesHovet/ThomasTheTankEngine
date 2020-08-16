//
//  EditorSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EditorSystem.hpp"
#include "allFamilies.h"
#include "libs/imgui/imgui.h"
#include "ImGuiUtils.hpp"
#include "EntityAdmin.hpp"
#include "DebugNameComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "IntersectionUtils.hpp"

GLuint EditorSystem::stem_VBO = 0;
GLuint EditorSystem::stem_VAO = 0;
GLuint EditorSystem::head_VBO = 0;
GLuint EditorSystem::head_VAO = 0;
Shader* gizmoShader;

float arrowStemVertsNoIndices[] = {
    0.0f,  0.0f,  1.0f,
    0.0f,  0.86603f,  -0.5f,
    0.0f, -0.86603f,  -0.5f,

    0.0f,  0.0f,  1.0f,
    0.0f, -0.86603f,  -0.5f,
    1.0f, -0.86603f,  -0.5f,

    0.0f,  0.0f,  1.0f,
    1.0f, -0.86603f,  -0.5f,
    1.0f,  0.0f,  1.0f,

    0.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,
    0.0f,  0.86603f,  -0.5f,

    1.0f,  0.0f,  1.0f,
    1.0f,  0.86603f,  -0.5f,
    0.0f,  0.86603f,  -0.5f,

    0.0f,  0.86603f,  -0.5f,
    1.0f,  0.86603f,  -0.5f,
    0.0f, -0.86603f,  -0.5f,

    1.0f,  0.86603f,  -0.5f,
    1.0f, -0.86603f,  -0.5f,
    0.0f, -0.86603f,  -0.5f,

    0.0f,  0.0f,  1.0f,
    0.0f, -0.86603f,  -0.5f,
    0.0f,  0.86603f,  -0.5f
};

float arrowVertsNoIndices[] = {
    0.0, 0.0, 2.0,
    0.0, 1.7321, -1,
    0.0, -1.7321, -1,

    0.0, 0.0, 2.0,
    0.0, -1.7321, -1,
    4.0, 0.0, 0.0,

    0.0, 0.0, 2.0,
    4.0, 0.0, 0.0,
    0.0, 1.7321, -1,

    4.0, 0.0, 0.0,
    0.0, -1.7321, -1,
    0.0, 1.7321, -1
};

void EditorSystem::init(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    edit.defaultEditorCameraComponent = CameraComponent();
    edit.defaultEditorCameraTransform = TransformComponent();
    edit.defaultEditorCameraTransform.m_orientation = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    edit.editorCameraComponent = edit.defaultEditorCameraComponent;
    edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    
    initRendering();
}

void EditorSystem::initRendering(){
    gizmoShader = &m_admin.m_ShaderCatalogSingleton.getShader("unlitOpaque");
    
    glGenVertexArrays(1, &stem_VAO);
    glBindVertexArray(stem_VAO);
    
    glGenBuffers(1, &stem_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, stem_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowStemVertsNoIndices), &arrowStemVertsNoIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &head_VAO);
    glBindVertexArray(head_VAO);
    
    glGenBuffers(1, &head_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, head_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowVertsNoIndices), &arrowVertsNoIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

    if(input.shouldSendKeysTo == KEY_INPUT_MODE::EDITOR){
        processEditorCameraKeyInput(camTransformC, dt, edit, input);
        
        if(input.hasPendingClick){
            ray r = input.getClickRaycast();
            entityID targetEID;
            glm::vec3 hit;
            if (getClosestOBBIntersectionEntity(r, &targetEID, &hit)){
                GreyBoxComponent * box = m_admin.tryGetComponent<GreyBoxComponent>(targetEID);
                box->m_color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
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
    renderImGui();
    renderGizmos();
}

void EditorSystem::renderAxesAtModelMat(glm::mat4 modelBase){
    GLuint modelLoc  = glGetUniformLocation(gizmoShader->ID, "model");
    
    // draw the three arrows
    glBindVertexArray(stem_VAO);
    glm::mat4 modelX = glm::scale(glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelX));
    gizmoShader->set4f("color", 1.0f, 0.0f, 0.0f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(head_VAO);
    glm::mat4 headModelX = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelX));
    gizmoShader->set4f("color", 1.0f, 0.0f, 0.0f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(stem_VAO);
    glm::mat4 modelY = glm::scale(glm::rotate(modelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelY));
    gizmoShader->set4f("color", 0.0f, 1.0f, 0.0f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(head_VAO);
    glm::mat4 headModelY = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelY));
    gizmoShader->set4f("color", 0.0f, 1.0f, 0.0f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(stem_VAO);
    glm::mat4 modelZ = glm::scale(glm::rotate(modelBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelZ));
    gizmoShader->set4f("color", 0.0f, 0.0f, 1.0f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(head_VAO);
    glm::mat4 headModelZ = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelZ));
    gizmoShader->set4f("color", 0.0f, 0.0f, 1.0f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
}

void EditorSystem::renderGizmos(){
    glDisable(GL_DEPTH_TEST);
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    gizmoShader->begin();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    GLuint viewLoc  = glGetUniformLocation(gizmoShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(gizmoShader->ID, "projection");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    
    for(GreyBoxFamilyStatic f : m_admin.getFamilyStaticVector<GreyBoxFamilyStatic>()){
        renderAxesAtModelMat(f.m_TransformComponent.getMat4());
    }

    gizmoShader->end();
    glEnable(GL_DEPTH_TEST);
}

void EditorSystem::renderImGui(){
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
    ImGui::End();
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
