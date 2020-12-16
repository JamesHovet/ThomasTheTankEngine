//
//  ImmediateRenderSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/16/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ImmediateRenderSystem.hpp"
#include "EntityAdmin.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "CameraFamily.hpp"


unsigned int ImmediateRenderSystem::lines_VBO = 0;
unsigned int ImmediateRenderSystem::lines_VAO = 0;
unsigned int ImmediateRenderSystem::tris_VBO = 0;
unsigned int ImmediateRenderSystem::tris_VAO = 0;

Shader* immModeShader;

void ImmediateRenderSystem::init(){
    immModeShader = &m_admin.m_ShaderCatalogSingleton.getShader("unlit_rgba");
    
    glGenVertexArrays(1, &lines_VAO);
    glBindVertexArray(lines_VAO);
    
    glGenBuffers(1, &lines_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ImmediateRenderSystem::render(){
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    drawLines();
    drawTris();

    imm.numLineVerts = 0;
    imm.immLineVerts.clear();
    imm.numTriVerts = 0;
    imm.immTriVerts.clear();
}

void ImmediateRenderSystem::drawLines(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    immModeShader->begin();
    
    GLuint viewLoc  = glGetUniformLocation(immModeShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(immModeShader->ID, "projection");
    GLuint modelLoc        = glGetUniformLocation(immModeShader->ID, "model");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    glm::mat4 identity = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
    
    glBindVertexArray(lines_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO);
    glBufferData(GL_ARRAY_BUFFER, imm.immLineVerts.size() * sizeof(float), &imm.immLineVerts[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, imm.numLineVerts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    immModeShader->end();
    
}

void ImmediateRenderSystem::drawTris(){
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
}
