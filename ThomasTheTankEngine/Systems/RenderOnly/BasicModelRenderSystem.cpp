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

#include "stb_image.h"

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
    
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
//    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE , glm::value_ptr(glm::mat4(1.0f)));
//
//    Model myModel = modelCatalog.getModel("lion");
//    Mesh lionhead = myModel.m_meshes[1];
//    Mesh lionbackground = myModel.m_meshes[0];
//    Material lionheadMat = myModel.m_materials[1];
//    Material lionbackgroundMat = myModel.m_materials[0];
//
//    if(lionheadMat.diffuseTextureName != ""){
//        glBindTexture(GL_TEXTURE_2D, modelCatalog.getTexture(lionheadMat.diffuseTextureName).m_textureID);
//    }
//    basicModelShader->set3f("diffuseColor", lionheadMat.diffuseColor);
//    glBindVertexArray(lionhead.m_VAO);
//    glDrawElements(GL_TRIANGLES, lionhead.numIndices, GL_UNSIGNED_INT, 0);
//
    
    /*
    int nrChannels;
//    // head
    int headWidth, headHeight;
    unsigned char * headData = stbi_load(FileUtils::getResourceAbsoluteFilePath("Textures/lion.tga").c_str(), &headWidth, &headHeight, &nrChannels, 0);

    if(! headData){
        std::cout << "errr" << std::endl;
    }

    unsigned int headTexture;
    glGenTextures(1, &headTexture);
    glBindTexture(GL_TEXTURE_2D, headTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, headWidth, headHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, headData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(headData);


    basicModelShader->set3f("diffuseColor", lionheadMat.diffuseColor);
    glBindVertexArray(lionhead.m_VAO);
    glDrawElements(GL_TRIANGLES, lionhead.numIndices, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //bk
    int bkWidth, bkHeight;
    unsigned char * bkData = stbi_load(FileUtils::getResourceAbsoluteFilePath("Textures/background.tga").c_str(), &bkWidth, &bkHeight, &nrChannels, 0);

    if(! bkData){
        std::cout << "errr" << std::endl;
    }

    unsigned int bkTexture;
    glGenTextures(1, &bkTexture);
    glBindTexture(GL_TEXTURE_2D, bkTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bkWidth, bkHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bkData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(bkData);

    basicModelShader->set3f("diffuseColor", lionbackgroundMat.diffuseColor);
    glBindVertexArray(lionbackground.m_VAO);
    glDrawElements(GL_TRIANGLES, lionbackground.numIndices, GL_UNSIGNED_INT, 0);
    */
    
    
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
            if(model.m_materials[i].diffuseTextureName != ""){
                basicModelShader->set1b("hasDiffuseTexture", true);
                glBindTexture(GL_TEXTURE_2D, modelCatalog.getTexture(model.m_materials[i].diffuseTextureName).m_textureID);
            } else {
                basicModelShader->set1b("hasDiffuseTexture", false);
            }
            glBindVertexArray(model.m_meshes[i].m_VAO);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(f.m_TransformComponent.getMat4()));
            glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(f.m_TransformComponent.getNormalMatrix()));
            glDrawElements(GL_TRIANGLES, model.m_meshes[i].numIndices, GL_UNSIGNED_INT, 0);
        }
    }
    
    basicModelShader->end();
}
