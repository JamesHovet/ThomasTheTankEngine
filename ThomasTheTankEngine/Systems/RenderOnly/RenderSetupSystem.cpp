//
//  RenderSetupSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/10/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "RenderSetupSystem.hpp"
#include "EntityAdmin.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

void RenderSetupSystem::init(){
    stbi_set_flip_vertically_on_load(true);
    
    m_admin.m_ShaderCatalogSingleton.registerShader("greybox", "Shaders/greybox.vert", "Shaders/greybox.frag");
    m_admin.m_ShaderCatalogSingleton.registerShader("unlitOpaque", "Shaders/unlitOpaque.vert", "Shaders/unlitOpaque.frag");
    m_admin.m_ShaderCatalogSingleton.registerShader("basic_model", "Shaders/basicModel.vert", "Shaders/basicModel.frag");
    m_admin.m_ShaderCatalogSingleton.registerShader("unlit_rgba", "Shaders/unlit_rgba.vert", "Shaders/unlit_rgba.frag");
    
    m_admin.m_ModelCatalogSingleton.registerModel("boxes", "Models/box_stack.obj");
    m_admin.m_ModelCatalogSingleton.registerModel("suzanne", "Models/suzanne.obj");
    m_admin.m_ModelCatalogSingleton.registerModel("multi-material", "Models/multi-material-test.obj");
    m_admin.m_ModelCatalogSingleton.registerModel("lion", "Models/sponzaLion.obj");
    m_admin.m_ModelCatalogSingleton.registerModel("column", "Models/column.obj");
    
}

void RenderSetupSystem::render(){
    setupCamera();
}

void RenderSetupSystem::setupCamera(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    
    if (m_admin.m_EditorSingleton.shouldUseEditorCamera){
        EditorSingleton& editorSingleton = m_admin.m_EditorSingleton;
        CameraComponent& cameraC = editorSingleton.editorCameraComponent;
        TransformComponent& transC = editorSingleton.editorCameraTransform;
        
        /*
        std::cout << "--\n";
        std::cout << SerializationUtils::serializeVec4(transC.m_position) << "\n";
        std::cout << SerializationUtils::serializeVec3(transC.getPosition3()) << "\n";
        std::cout << SerializationUtils::serializeVec3(transC.getForward3()) << "\n";
        std::cout << SerializationUtils::serializeVec3(transC.getUp3()) << std::endl;
         */
        
        renderSingleton.view = glm::lookAt(transC.getPosition3(), transC.getPosition3() + transC.getForward3(), transC.getUp3());
        renderSingleton.projection = glm::perspective(glm::radians(cameraC.m_FOV), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
        renderSingleton.currentCameraC = &cameraC;
        renderSingleton.currentCameraTransformC = &transC;
    } else {
        std::unordered_map<entityID, CameraFamily>& cameraFamilies = m_admin.getFamilyMap<CameraFamily>();
        for(auto it = cameraFamilies.begin(); it != cameraFamilies.end(); it++){
            CameraComponent& cameraC = it->second.m_CameraComponent;
            TransformComponent& transC = it->second.m_TransformComponent;
            if(cameraC.m_enabled){
                renderSingleton.currentCameraC = &cameraC;
                renderSingleton.currentCameraTransformC = &transC;
                
                renderSingleton.view = glm::lookAt(transC.getPosition3(), transC.getPosition3() + transC.getForward3(), transC.getUp3());
                renderSingleton.projection = glm::perspective(glm::radians(cameraC.m_FOV), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
                break;
            }
        }
    }
}
