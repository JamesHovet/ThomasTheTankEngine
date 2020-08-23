//
//  GreyBoxRenderSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "GreyBoxFamily.hpp"
#include "CameraFamily.hpp"

#include "GreyBoxRenderSystem.hpp"
#include "EntityAdmin.hpp"

#include <vector>

unsigned int GreyBoxRenderSystem::cube_VBO    = 0;
unsigned int GreyBoxRenderSystem::cube_VAO    = 0;
unsigned int GreyBoxRenderSystem::instanceVBO = 0;
Shader* greyBoxShader;

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
    glm::vec4 rotation;
};
#pragma pack(pop)

void GreyBoxRenderSystem::init(){
    greyBoxShader = &m_admin.m_ShaderCatalogSingleton.getShader("greybox");
    
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
    
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(greyBoxRenderData), (void *) offsetof(greyBoxRenderData, rotation));
    glEnableVertexAttribArray(4); // location 3 : v4 color (rgba)
    glVertexAttribDivisor(4, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
}

void GreyBoxRenderSystem::render(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    
    greyBoxShader->begin();
    bool shouldDrawWireframe = false;
    if(shouldDrawWireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    GLuint viewLoc  = glGetUniformLocation(greyBoxShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(greyBoxShader->ID, "projection");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
     
    
    // populate the instance positions array
    std::vector<GreyBoxFamilyStatic>& boxes = m_admin.getFamilyStaticVector<GreyBoxFamilyStatic>();
    
    int numBoxes = boxes.size();
//    numBoxes = 3;
    
    greyBoxRenderData instanceData[numBoxes];
    
    for(int i = 0; i < numBoxes; i++){
        
        instanceData[i].position = boxes[i].m_TransformComponent.m_position;
        instanceData[i].scale    = boxes[i].m_TransformComponent.m_scale;
        instanceData[i].color    = boxes[i].m_GreyBoxComponent.m_color;
        auto orient = boxes[i].m_TransformComponent.m_orientation;
        instanceData[i].rotation = glm::vec4(orient.x, orient.y, orient.z, orient.w);
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(greyBoxRenderData) * numBoxes, &instanceData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(cube_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, numBoxes);
    glBindVertexArray(0);
    
    greyBoxShader->end();
    
}
