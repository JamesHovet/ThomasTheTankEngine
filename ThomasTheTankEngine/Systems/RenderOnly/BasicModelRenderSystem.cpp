//
//  BasicModelRenderSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BasicModelRenderSystem.hpp"
#include "EntityAdmin.hpp"

Shader* basicModelShader;

void BasicModelRenderSystem::init(){
    m_admin.m_ModelCatalogSingleton.registerModel("boxes", "Models/box_stack.obj");
    basicModelShader = &m_admin.m_ShaderCatalogSingleton.getShader("basic_model");
}

void BasicModelRenderSystem::render(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    
    basicModelShader->begin();
    GLuint modelLoc      = glGetUniformLocation(basicModelShader->ID, "model");
    GLuint viewLoc       = glGetUniformLocation(basicModelShader->ID, "view");
    GLuint projectionLoc = glGetUniformLocation(basicModelShader->ID, "projection");

    glm::mat4 identity = glm::mat4(1.0f);
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    glm::mat4 model = glm::mat4(1.0f);
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
//    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    
    
    Model myModel = m_admin.m_ModelCatalogSingleton.getModel("boxes");
    Mesh  myMesh = myModel.m_meshes[2];
    
    for(int i = 0; i < 3; i++){
        glBindVertexArray(myModel.m_meshes[i].m_VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    basicModelShader->end();
}
