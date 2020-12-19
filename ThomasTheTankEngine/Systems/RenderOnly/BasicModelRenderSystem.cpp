//
//  BasicModelRenderSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include <GL/glew.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BasicModelRenderSystem.hpp"
#include "BasicModelFamily.hpp"
#include "EntityAdmin.hpp"



Shader* basicModelShader;

void BasicModelRenderSystem::init(){
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
    
    basicModelShader->set3f("viewPos", renderSingleton.currentCameraTransformC->getPosition3());
    basicModelShader->set3f("lightColor", glm::vec3(1.0f));
    basicModelShader->set1f("time", ((float)SDL_GetTicks() / 1000.0f));
    
    GLuint modelLoc        = glGetUniformLocation(basicModelShader->ID, "model");
    GLuint viewLoc         = glGetUniformLocation(basicModelShader->ID, "view");
    GLuint projectionLoc   = glGetUniformLocation(basicModelShader->ID, "projection");
    GLuint normalMatrixLoc = glGetUniformLocation(basicModelShader->ID, "normalMatrix");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    
    std::vector<BasicModelFamilyStatic> families = m_admin.getFamilyStaticVector<BasicModelFamilyStatic>();

    for (BasicModelFamilyStatic f : families){
        std::string model_name = f.m_BasicModelComponent.m_model_name;
        if(model_name == ""){
            continue;
        }
        if(!modelCatalog.modelExists(model_name)){
            std::cerr << "Could not find model for " << model_name << std::endl;
            std::cout << "Could not find model for " << model_name << std::endl;
            continue;
        }
        Model model = modelCatalog.getModel(model_name);
        for(int i = 0; i < model.m_numMeshes; i++){
            Material thisMaterial = model.m_materials[i];
            Mesh thisMesh = model.m_meshes[i];
            
            basicModelShader->set3f("diffuseColor", thisMaterial.diffuseColor);
            basicModelShader->set3f("specularColor", thisMaterial.specularColor);
            basicModelShader->set1f("specularExp", thisMaterial.specularExponent);
            basicModelShader->set3f("ambientColor", thisMaterial.ambientColor);
            
            if(thisMaterial.diffuseTextureName != ""){
                basicModelShader->set1b("hasDiffuseTexture", true);
                basicModelShader->set1i("diffuseTexture", 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, modelCatalog.getTexture(thisMaterial.diffuseTextureName).m_textureID);
            } else {
                basicModelShader->set1b("hasDiffuseTexture", false);
            }
            
            if(thisMaterial.normalTextureName != ""){
                basicModelShader->set1b("hasNormalMap", true);
                basicModelShader->set1i("normalTexture", 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, modelCatalog.getTexture(thisMaterial.normalTextureName).m_textureID);
            } else {
                basicModelShader->set1b("hasNormalMap", false);
            }
            
            glBindVertexArray(thisMesh.m_VAO);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(f.m_TransformComponent.m_cachedMat4));
            glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(f.m_TransformComponent.getNormalMatrix()));
            glDrawElements(GL_TRIANGLES, thisMesh.m_numIndices, GL_UNSIGNED_INT, 0);
        }
    }
    
    basicModelShader->end();
}
