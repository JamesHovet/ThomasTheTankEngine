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
#include "BasicModelFamily.hpp"
#include "EntityAdmin.hpp"

Shader* basicModelShader;

void BasicModelRenderSystem::init(){
    m_admin.m_ModelCatalogSingleton.registerModel("boxes", "Models/box_stack.obj");
    m_admin.m_ModelCatalogSingleton.registerModel("suzanne", "Models/suzanne.obj");
    m_admin.m_ModelCatalogSingleton.registerModel("multi-material", "Models/multi-material-test.obj");
//    m_admin.m_ModelCatalogSingleton.registerModel("sponza", "Models/sponza.obj");
    m_admin.m_ModelCatalogSingleton.registerModel("lion", "Models/sponzaLion.obj");
    
    basicModelShader = &m_admin.m_ShaderCatalogSingleton.getShader("basic_model");
}

void BasicModelRenderSystem::render(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    ModelCatalogSingleton& modelCatalog = m_admin.m_ModelCatalogSingleton;
    
    basicModelShader->begin();
    
    bool shouldDrawWireframe = false;
    if(shouldDrawWireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT, GL_FILL);
    }
    
    
    basicModelShader->set3f("viewPos", renderSingleton.currentCameraTransformC->m_position);
    GLuint modelLoc        = glGetUniformLocation(basicModelShader->ID, "model");
    GLuint viewLoc         = glGetUniformLocation(basicModelShader->ID, "view");
    GLuint projectionLoc   = glGetUniformLocation(basicModelShader->ID, "projection");
    GLuint normalMatrixLoc = glGetUniformLocation(basicModelShader->ID, "normalMatrix");

//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    
    std::vector<BasicModelFamilyStatic> families = m_admin.getFamilyStaticVector<BasicModelFamilyStatic>();

    for (BasicModelFamilyStatic f : families){
        std::string model_name = f.m_BasicModelComponent.m_model_name;
        if(!modelCatalog.modelExists(model_name)){
            std::cerr << "Could not find model for " << model_name << std::endl;
            std::cout << "Could not find model for " << model_name << std::endl;
            continue;
        }
        Model model = modelCatalog.getModel(model_name);
        for(int i = 0; i < model.m_numMeshes; i++){
            basicModelShader->set3f("diffuseColor", model.m_materials[i].diffuseColor);
            glBindVertexArray(model.m_meshes[i].m_VAO);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(f.m_TransformComponent.getMat4()));
            glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(f.m_TransformComponent.getNormalMatrix()));
            glDrawElements(GL_TRIANGLES, model.m_meshes[i].numIndices, GL_UNSIGNED_INT, 0);
        }
    }
    
    basicModelShader->end();
}
