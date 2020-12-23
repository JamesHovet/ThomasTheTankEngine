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


unsigned int ImmediateRenderSystem::lines_VBO3d = 0;
unsigned int ImmediateRenderSystem::lines_VAO3d = 0;
unsigned int ImmediateRenderSystem::tris_VBO3d = 0;
unsigned int ImmediateRenderSystem::tris_VAO3d = 0;
unsigned int ImmediateRenderSystem::lines_VBO2d = 0;
unsigned int ImmediateRenderSystem::lines_VAO2d = 0;
unsigned int ImmediateRenderSystem::tris_VBO2d = 0;
unsigned int ImmediateRenderSystem::tris_VAO2d = 0;

Shader* immModeShader;

void ImmediateRenderSystem::init(){
    immModeShader = &m_admin.m_ShaderCatalogSingleton.getShader("unlit_rgba");
    
    // lines 3d
    glGenVertexArrays(1, &lines_VAO3d);
    glBindVertexArray(lines_VAO3d);
    
    glGenBuffers(1, &lines_VBO3d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO3d);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Tris 3d
    glGenVertexArrays(1, &tris_VAO3d);
    glBindVertexArray(tris_VAO3d);

    glGenBuffers(1, &tris_VBO3d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO3d);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // lines 2d
    glGenVertexArrays(1, &lines_VAO2d);
    glBindVertexArray(lines_VAO2d);
    
    glGenBuffers(1, &lines_VBO2d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO2d);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Tris 2d
    glGenVertexArrays(1, &tris_VAO2d);
    glBindVertexArray(tris_VAO2d);

    glGenBuffers(1, &tris_VBO2d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO2d);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ImmediateRenderSystem::render(){
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    draw3d();

    imm.numLineVerts3d = 0;
    imm.immLineVerts3d.clear();
    imm.numTriVerts3d = 0;
    imm.immTriVerts3d.clear();
    
    draw2d();

    imm.numLineVerts2d = 0;
    imm.immLineVerts2d.clear();
    imm.numTriVerts2d = 0;
    imm.immTriVerts2d.clear();
}

void ImmediateRenderSystem::draw3d(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    immModeShader->begin();
    
    GLuint viewLoc        = glGetUniformLocation(immModeShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(immModeShader->ID, "projection");
    GLuint modelLoc       = glGetUniformLocation(immModeShader->ID, "model");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    glm::mat4 identity = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
    
    glBindVertexArray(lines_VAO3d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO3d);
    glBufferData(GL_ARRAY_BUFFER, imm.immLineVerts3d.size() * sizeof(float), &imm.immLineVerts3d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, imm.numLineVerts3d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(tris_VAO3d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO3d);
    glBufferData(GL_ARRAY_BUFFER, imm.immTriVerts3d.size() * sizeof(float), &imm.immTriVerts3d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, imm.numTriVerts3d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    immModeShader->end();
    
}

void ImmediateRenderSystem::draw2d(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    immModeShader->begin();
    
    GLuint viewLoc        = glGetUniformLocation(immModeShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(immModeShader->ID, "projection");
    GLuint modelLoc       = glGetUniformLocation(immModeShader->ID, "model");
    
//    renderSingleton.view = glm::lookAt(transC.getPosition3(), transC.getPosition3() + transC.getForward3(), transC.getUp3());
//    renderSingleton.projection = glm::perspective(glm::radians(cameraC.m_FOV), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
    
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 orthoProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(identity));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(orthoProjection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
    
    glBindVertexArray(lines_VAO2d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO2d);
    glBufferData(GL_ARRAY_BUFFER, imm.immLineVerts2d.size() * sizeof(float), &imm.immLineVerts2d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, imm.numLineVerts2d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(tris_VAO2d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO2d);
    glBufferData(GL_ARRAY_BUFFER, imm.immTriVerts2d.size() * sizeof(float), &imm.immTriVerts2d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, imm.numTriVerts2d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    immModeShader->end();
    
}
