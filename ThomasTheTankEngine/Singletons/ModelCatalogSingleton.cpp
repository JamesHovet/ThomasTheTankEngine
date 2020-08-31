//
//  ModelCatalogSingleton.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ModelCatalogSingleton.hpp"
#include "OBJ_Loader.h"
#include "FileUtils.hpp"
#include <algorithm> // for min()

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "stb_image.h"

using path = boost::filesystem::path;

ModelCatalogSingleton::ModelCatalogSingleton(){
    
}

ModelCatalogSingleton::~ModelCatalogSingleton(){
    
}

Model& ModelCatalogSingleton::getModel(std::string name){
    assert(m_modelCatalogEntries.count(name) != 0);
    return m_modelCatalogEntries.at(name).m_model;
}

Texture& ModelCatalogSingleton::getTexture(std::string name){
    assert(m_textureCatalogEntries.count(name) != 0);
    return m_textureCatalogEntries.at(name).m_texture;
}

//TODO: materials!
bool ModelCatalogSingleton::registerModel(std::string name, const char * objPathRelativeCStr){
    
    ModelCatalogEntry& modelEntry = m_modelCatalogEntries.emplace(name, ModelCatalogEntry()).first->second; // boy is this stupid syntax
    
    modelEntry.objFileAbsolute = boost::filesystem::path(FileUtils::getResourceAbsoluteFilePath(objPathRelativeCStr));
    modelEntry.lastTimeModified = boost::filesystem::last_write_time(modelEntry.objFileAbsolute);
    
    
    objl::Loader Loader;
    bool loadout = Loader.LoadFile(modelEntry.objFileAbsolute.c_str());
    
    if(!loadout){
        std::cerr << "Could not load file: " << modelEntry.objFileAbsolute.c_str() << std::endl;
        return false;
    }
    
    modelEntry.m_model.m_numMeshes = std::min((size_t) MAX_MESHES, Loader.LoadedMeshes.size());
    for(int i = 0; i < std::min((size_t) MAX_MESHES, Loader.LoadedMeshes.size()); i++){
        objl::Mesh curMeshIn = Loader.LoadedMeshes[i];
        Mesh& curMeshOut = modelEntry.m_model.m_meshes[i];
        objl::Material curMaterialIn = curMeshIn.MeshMaterial;
        Material& curMaterialOut = modelEntry.m_model.m_materials[i];
        
        // handle textures
        path baseTexturePath;
        std::string baseTexturePathString = curMaterialIn.map_Kd;
        if(baseTexturePathString != ""){
            std::size_t found = baseTexturePathString.rfind("Resources");
            if(found != std::string::npos){
                std::string substr = baseTexturePathString.substr(found + 10);
                baseTexturePath = path(FileUtils::getResourceAbsoluteFilePath(substr));
                curMaterialOut.diffuseTextureName = substr;
            } else {
                printf("using dangerous absolute file path\n");
                baseTexturePath = path(baseTexturePathString);
                curMaterialOut.diffuseTextureName = baseTexturePath.filename().string();
            }
            if(! boost::filesystem::exists(baseTexturePath)){
                std::cerr << "Missing Texture: " << baseTexturePath.c_str() << std::endl;
                std::cout << "Missing Texture: " << baseTexturePath.c_str() << std::endl;
                return false;
            }
            
            if(m_textureCatalogEntries.count(curMaterialOut.diffuseTextureName) == 0){
                
                unsigned char * data;
                // diffuse
                
                Texture curTextureOut;
                data = stbi_load(baseTexturePath.c_str(), &curTextureOut.m_width, &curTextureOut.m_height, &curTextureOut.m_numChannels, 0);
                if(!data){
                    std::cout << "could not load image data from:" << baseTexturePath << std::endl;
                    stbi_image_free(data);
                    return false;
                }
                switch (curTextureOut.m_numChannels) {
                    case 1:
                        curTextureOut.m_storage_format = GL_RED;
                        break;
                    case 2:
                        curTextureOut.m_storage_format = GL_RG;
                        break;
                    case 3:
                        curTextureOut.m_storage_format = GL_RGB;
                        break;
                    case 4:
                        curTextureOut.m_storage_format = GL_RGBA;
                        break;
                    default:
                        std::cout << "Invalid number of image channels" << std::endl;
                        return false;
                        break;
                }
                
                glGenTextures(1, &curTextureOut.m_textureID);
                glBindTexture(GL_TEXTURE_2D, curTextureOut.m_textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, curTextureOut.m_storage_format, curTextureOut.m_width, curTextureOut.m_height, 0, curTextureOut.m_storage_format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(data);
                
                TextureCatalogEntry& diffuseTextureEntry = m_textureCatalogEntries.emplace(curMaterialOut.diffuseTextureName, TextureCatalogEntry()).first->second;
                diffuseTextureEntry.m_texture = curTextureOut;
                diffuseTextureEntry.lastTimeModified = boost::filesystem::last_write_time(baseTexturePath);
                diffuseTextureEntry.textureFileAbsolute = baseTexturePath;
            } else {
//                std::cout << "already loaded " << baseTexturePath;
            }
        }
        // handle properties
        curMaterialOut.name = curMaterialIn.name;
        curMaterialOut.ambientColor = glm::vec3(curMaterialIn.Ka.X, curMaterialIn.Ka.Y, curMaterialIn.Ka.Z);
        curMaterialOut.diffuseColor = glm::vec3(curMaterialIn.Kd.X, curMaterialIn.Kd.Y, curMaterialIn.Kd.Z);
        curMaterialOut.specularColor = glm::vec3(curMaterialIn.Ks.X, curMaterialIn.Ks.Y, curMaterialIn.Ks.Z);
        curMaterialOut.specularExponent = curMaterialIn.Ns;

        float* vertsInPtr = &curMeshIn.Vertices[0].Position.X;
        unsigned int* indicesInPtr = &curMeshIn.Indices[0];
        
        glGenVertexArrays(1, &curMeshOut.m_VAO);
        glGenBuffers(1, &curMeshOut.m_VBO);
        glGenBuffers(1, &curMeshOut.m_EBO);
        curMeshOut.numIndices = curMeshIn.Vertices.size();
        
        glBindVertexArray(curMeshOut.m_VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, curMeshOut.m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * curMeshIn.Vertices.size(), vertsInPtr, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curMeshOut.m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * curMeshIn.Indices.size(), indicesInPtr, GL_STATIC_DRAW);
        
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    
    return true;
}
