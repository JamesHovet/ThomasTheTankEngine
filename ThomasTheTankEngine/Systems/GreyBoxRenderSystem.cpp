//
//  GreyBoxRenderSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "GreyBoxRenderSystem.hpp"
#include "EntityAdmin.hpp"

unsigned int GreyBoxRenderSystem::cube_VBO = 0;
unsigned int GreyBoxRenderSystem::cube_VAO = 0;
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

void GreyBoxRenderSystem::init(){
    greyboxShader = Shader("Shaders/greybox.vert", "Shaders/greybox.frag");
    
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);
    
    glGenBuffers(1, &cube_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(greyBoxVertsNoIndices), greyBoxVertsNoIndices, GL_STATIC_DRAW);
    
    // not sure I need this...
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    bool shouldDrawWireframe = false;
    if(shouldDrawWireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
}

//TODO: @Cleanup: factor these sorts of globals out

void GreyBoxRenderSystem::render(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    
    
    { // setup camera
        
        std::unordered_map<entityID, CameraFamily>& cameraFamilies = m_admin.getFamilyMap<CameraFamily>();
        for(auto it = cameraFamilies.begin(); it != cameraFamilies.end(); it++){
            CameraComponent& cameraC = it->second.m_CameraComponent;
            TransformComponent& transC = it->second.m_TransformComponent;
            if(cameraC.m_enabled){
                renderSingleton.view = glm::lookAt(transC.m_position, transC.m_position + cameraC.m_forward, cameraC.m_up);
                renderSingleton.projection = glm::perspective(glm::radians(cameraC.m_FOV), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
                break;
            }
        }
        
//        m_admin.m_RenderSingleton.view = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -3.0f)), -15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//        m_admin.m_RenderSingleton.projection = glm::perspective(glm::radians(45.0f), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
    }
    
    glm::mat4 model = glm::mat4(1.0f);
    
    greyboxShader.begin();
    
    GLuint modelLoc = glGetUniformLocation(greyboxShader.ID, "model");
    GLuint viewLoc  = glGetUniformLocation(greyboxShader.ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(greyboxShader.ID, "projection");
    
     
    //TODO: Factor out basically everything from this very hard-coded initial step.
    
    for (std::pair<entityID, GreyBoxFamily> p : m_admin.getFamilyMap<GreyBoxFamily>()){
        GreyBoxFamily f = p.second;
        glm::vec3 pos = f.m_TransformComponent.m_position;
        
//        printf("[%4d]:(%4.0f, %4.0f, %4.0f) : %2.2f\n", f.eID, pos.x, pos.y, pos.z, f.m_GreyBoxComponent.m_color.r);
        
        model = glm::translate(glm::mat4(1.0f), pos);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
        
        glm::vec3 color = f.m_GreyBoxComponent.m_color;
        greyboxShader.set3f("color", color.r, color.g, color.b);
        
        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
    
    greyboxShader.end();
    
}
