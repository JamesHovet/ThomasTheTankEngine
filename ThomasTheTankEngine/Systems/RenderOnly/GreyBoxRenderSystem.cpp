//
//  GreyBoxRenderSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "GreyBoxRenderSystem.hpp"
#include "EntityAdmin.hpp"
#include <vector>

unsigned int GreyBoxRenderSystem::cube_VBO    = 0;
unsigned int GreyBoxRenderSystem::cube_VAO    = 0;
unsigned int GreyBoxRenderSystem::instanceVBO = 0;
Shader greyboxShader;

float greyBoxVertsNoIndices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

#pragma pack(push, 0)
struct greyBoxRenderData {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec4 color;
};
#pragma pack(pop)

void GreyBoxRenderSystem::init(){
    greyboxShader = Shader("Shaders/greybox.vert", "Shaders/greybox.frag");
    
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);
    
    glGenBuffers(1, &cube_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(greyBoxVertsNoIndices), &greyBoxVertsNoIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &instanceVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(greyBoxRenderData), (void *) offsetof(greyBoxRenderData, position));
    glEnableVertexAttribArray(1); // location 1 : vec3 ipos
    glVertexAttribDivisor(1, 1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(greyBoxRenderData), (void *) offsetof(greyBoxRenderData, scale));
    glEnableVertexAttribArray(2); // location 2 : vec3 scale
    glVertexAttribDivisor(2, 1);
//
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(greyBoxRenderData), (void *) offsetof(greyBoxRenderData, color));
    glEnableVertexAttribArray(3); // location 3 : v4 color (rgba)
    glVertexAttribDivisor(3, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    bool shouldDrawWireframe = false;
    if(shouldDrawWireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
}

void GreyBoxRenderSystem::setupCamera(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    
    if (m_admin.m_EditorSingleton.shouldUseEditorCamera){
        EditorSingleton& editorSingleton = m_admin.m_EditorSingleton;
        CameraComponent& cameraC = editorSingleton.editorCameraComponent;
        TransformComponent& transC = editorSingleton.editorCameraTransform;
        
        renderSingleton.view = glm::lookAt(transC.m_position, transC.m_position + cameraC.m_forward, cameraC.m_up);
        renderSingleton.projection = glm::perspective(glm::radians(cameraC.m_FOV), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
    } else {
        std::unordered_map<entityID, CameraFamily>& cameraFamilies = m_admin.getFamilyMap<CameraFamily>();
        for(auto it = cameraFamilies.begin(); it != cameraFamilies.end(); it++){
            CameraComponent& cameraC = it->second.m_CameraComponent;
            TransformComponent& transC = it->second.m_TransformComponent;
            if(cameraC.m_enabled){
                renderSingleton.currentCameraC = &cameraC;
                renderSingleton.currentCameraTransformC = &transC;
                
                renderSingleton.view = glm::lookAt(transC.m_position, transC.m_position + cameraC.m_forward, cameraC.m_up);
                renderSingleton.projection = glm::perspective(glm::radians(cameraC.m_FOV), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
                break;
            }
        }
    }
}

void GreyBoxRenderSystem::render(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    
    setupCamera();
    greyboxShader.begin();

    GLuint viewLoc  = glGetUniformLocation(greyboxShader.ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(greyboxShader.ID, "projection");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
     
    
    // populate the instance positions array
    std::vector<GreyBoxFamilyStatic>& boxes = m_admin.getFamilyStaticVector<GreyBoxFamilyStatic>();
    
    auto numBoxes = boxes.size();
//    numBoxes = 3;
    
    greyBoxRenderData instanceData[numBoxes];
    
    for(int i = 0; i < numBoxes; i++){
        
        instanceData[i].position = boxes[i].m_TransformComponent.m_position;
        instanceData[i].scale    = boxes[i].m_TransformComponent.m_scale;
        instanceData[i].color    = boxes[i].m_GreyBoxComponent.m_color;
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(greyBoxRenderData) * numBoxes, &instanceData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(cube_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, numBoxes);
    glBindVertexArray(0);
    
    greyboxShader.end();
    
}
